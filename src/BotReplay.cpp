#include "BotReplay.hpp"

BotReplay* BotReplay::get() {
    static BotReplay instance;
    return &instance;
}

void BotReplay::reset() {
    m_frames.clear();
    m_playIndex = 0;
    m_recording = false;
    m_playing = false;
}

void BotReplay::startRecording() {
    m_frames.clear();
    m_recording = true;
    m_playing = false;
}

void BotReplay::stopRecording() {
    m_recording = false;
}

void BotReplay::startPlaying() {
    if (m_frames.empty()) {
        Notification::create("Belum ada replay yang direkam!", NotificationIcon::Error)->show();
        return;
    }
    m_playIndex = 0;
    m_playing = true;
    m_recording = false;
}

void BotReplay::stopPlaying() {
    m_playing = false;
}

void BotReplay::onInput(int button, bool player2, bool down, int frame) {
    if (!m_recording) return;
    m_frames.push_back({frame, button, player2, down});
}

void BotReplay::tick(int frame) {
    if (!m_playing) return;

    auto playLayer = PlayLayer::get();
    if (!playLayer) return;

    // Fire every recorded event scheduled for this frame (there can be
    // more than one input on the same physics tick).
    while (m_playIndex < m_frames.size() && m_frames[m_playIndex].frame <= frame) {
        auto& ev = m_frames[m_playIndex];

        auto player = ev.player2 ? playLayer->m_player2 : playLayer->m_player1;
        if (player) {
            if (ev.down) {
                player->pushButton(static_cast<PlayerButton>(ev.button));
            } else {
                player->releaseButton(static_cast<PlayerButton>(ev.button));
            }
        }
        m_playIndex++;
    }

    // Reached the end of the macro -> stop automatically.
    if (m_playIndex >= m_frames.size()) {
        m_playing = false;
    }
}

bool BotReplay::saveToFile(const std::filesystem::path& path) {
    std::ofstream out(path, std::ios::binary);
    if (!out.is_open()) return false;

    size_t count = m_frames.size();
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));
    out.write(reinterpret_cast<const char*>(m_frames.data()), count * sizeof(ReplayFrame));
    return true;
}

bool BotReplay::loadFromFile(const std::filesystem::path& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in.is_open()) return false;

    size_t count = 0;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));
    if (!in) return false;

    m_frames.resize(count);
    in.read(reinterpret_cast<char*>(m_frames.data()), count * sizeof(ReplayFrame));
    return static_cast<bool>(in) || in.eof();
}
