#pragma once
#include <Geode/Geode.hpp>
#include <vector>
#include <cstdint>
#include <fstream>

using namespace geode::prelude;

// One recorded input event: at which physics frame, which button,
// pressed or released, on which player (1 = p1, 2 = p2/dual).
struct ReplayFrame {
    int frame;
    int button;   // 1 = jump, 2 = left, 3 = right
    bool player2;
    bool down;    // true = push, false = release
};

// Simple frame-perfect macro recorder + player for a single attempt.
// Works by hooking PlayerObject::pushButton / releaseButton while recording
// (captures real input) and PlayLayer::update while playing back
// (re-fires the same button calls at the same physics frame count).
class BotReplay {
public:
    static BotReplay* get();

    void startRecording();
    void stopRecording();
    void startPlaying();
    void stopPlaying();

    void reset();

    bool isRecording() const { return m_recording; }
    bool isPlaying() const { return m_playing; }

    // Called every physics tick from PlayLayer::update hook
    void tick(int frame);

    // Called from PlayerObject push/release hooks while recording
    void onInput(int button, bool player2, bool down, int frame);

    bool saveToFile(const std::filesystem::path& path);
    bool loadFromFile(const std::filesystem::path& path);

    size_t frameCount() const { return m_frames.size(); }

private:
    std::vector<ReplayFrame> m_frames;
    size_t m_playIndex = 0;
    bool m_recording = false;
    bool m_playing = false;
};
