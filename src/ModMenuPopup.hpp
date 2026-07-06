#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

enum class ModTab {
    Level = 0,
    Player = 1,
    Cosmetic = 2,
    BotReplayTab = 3
};

// NOTE: in current Geode (v5.7.1), geode::Popup is a plain class, not a
// template. You override init(...) directly (calling Popup::init(w, h)
// first), there is no separate setup() step.
class ModMenuPopup : public geode::Popup {
protected:
    CCMenu* m_tabMenu = nullptr;
    CCNode* m_contentLayer = nullptr;
    ModTab m_currentTab = ModTab::Level;

    bool init() override;

    void selectTab(ModTab tab);
    void onTabButton(CCObject* sender);

    void buildLevelTab();
    void buildPlayerTab();
    void buildCosmeticTab();
    void buildBotReplayTab();

    void clearContent();

public:
    static ModMenuPopup* create();
};
