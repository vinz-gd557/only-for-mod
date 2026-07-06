#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include "BotReplay.hpp"

using namespace geode::prelude;

// Track physics frame count ourselves (GD's internal frame counter isn't
// always exposed the same way across versions, so we keep our own).
static int g_frameCount = 0;

class $modify(BRPlayLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;
        g_frameCount = 0;
        // Fresh attempt: if we're about to play back a macro, rewind it.
        if (BotReplay::get()->isPlaying()) {
            BotReplay::get()->stopPlaying();
            BotReplay::get()->startPlaying();
        }
        return true;
    }

    void update(float dt) {
        PlayLayer::update(dt);
        g_frameCount++;
        BotReplay::get()->tick(g_frameCount);
    }

    void resetLevel() {
        PlayLayer::resetLevel();
        g_frameCount = 0;
        if (BotReplay::get()->isPlaying()) {
            // restart macro from the beginning on every retry
            BotReplay::get()->startPlaying();
        }
    }
};

class $modify(BRPlayerObject, PlayerObject) {
    void pushButton(PlayerButton button) {
        PlayerObject::pushButton(button);

        if (BotReplay::get()->isRecording()) {
            auto playLayer = PlayLayer::get();
            bool isP2 = playLayer && this == playLayer->m_player2;
            BotReplay::get()->onInput(static_cast<int>(button), isP2, true, g_frameCount);
        }
    }

    void releaseButton(PlayerButton button) {
        PlayerObject::releaseButton(button);

        if (BotReplay::get()->isRecording()) {
            auto playLayer = PlayLayer::get();
            bool isP2 = playLayer && this == playLayer->m_player2;
            BotReplay::get()->onInput(static_cast<int>(button), isP2, false, g_frameCount);
        }
    }
};
