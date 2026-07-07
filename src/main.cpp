#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/LevelSelectLayer.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include "FloatingButton.hpp"

using namespace geode::prelude;

// Shows the floating button on most menu/navigation screens, but
// deliberately NOT inside PlayLayer itself - it stays out of the way
// during actual live gameplay, and is reachable again as soon as you
// pause (PauseLayer) or go back to any menu.
static void attachFloatingButton(CCNode* layer) {
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

class $modify(FBCreatorLayer, CreatorLayer) {
    bool init() {
        if (!CreatorLayer::init()) return false;
        attachFloatingButton(this);
        return true;
    }
};

class $modify(FBGJGarageLayer, GJGarageLayer) {
    bool init() {
        if (!GJGarageLayer::init()) return false;
        attachFloatingButton(this);
        return true;
    }
};

$on_mod(Loaded) {
    log::info("Floating Menu Mod loaded!");
}
