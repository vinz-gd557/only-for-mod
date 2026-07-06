#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/LevelSelectLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include "FloatingButton.hpp"

using namespace geode::prelude;

// Attach the floating button to whatever CCLayer we're given, unless
// it's already there. This is more reliable than trying to hook
// CCDirector's scene transitions, since GD wraps a lot of scene changes
// in CCTransitionScene which makes "the running scene" ambiguous.
static void attachFloatingButton(CCLayer* layer) {
    if (!layer) return;
    if (layer->getChildByID("floating-menu-button"_spr)) return;

    auto btn = FloatingButton::create();
    btn->setID("floating-menu-button"_spr);
    btn->setZOrder(1000);
    layer->addChild(btn);
}

class $modify(FBMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;
        attachFloatingButton(this);
        return true;
    }
};

class $modify(FBPlayLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;
        attachFloatingButton(this);
        return true;
    }
};

class $modify(FBPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();
        attachFloatingButton(this);
    }
};

class $modify(FBLevelSelectLayer, LevelSelectLayer) {
    bool init(int page) {
        if (!LevelSelectLayer::init(page)) return false;
        attachFloatingButton(this);
        return true;
    }
};

class $modify(FBLevelInfoLayer, LevelInfoLayer) {
    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) return false;
        attachFloatingButton(this);
        return true;
    }
};

$on_mod(Loaded) {
    log::info("Floating Menu Mod loaded!");
}
