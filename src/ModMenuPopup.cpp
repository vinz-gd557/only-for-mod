#include "ModMenuPopup.hpp"
#include "BotReplay.hpp"

ModMenuPopup* ModMenuPopup::create() {
    auto ret = new ModMenuPopup();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool ModMenuPopup::init() {
    if (!Popup::init(320.f, 240.f)) return false;

    this->setTitle("Floating Menu Mod");

    // --- Tab bar ---
    m_tabMenu = CCMenu::create();
    m_tabMenu->setPosition({0.f, 0.f});

    const char* names[4] = {"Level", "Player", "Cosmetic", "Bot Replay"};
    float startX = m_mainLayer->getContentSize().width / 2 - 135.f;
    float y = m_mainLayer->getContentSize().height - 40.f;

    for (int i = 0; i < 4; i++) {
        auto label = CCLabelBMFont::create(names[i], "bigFont.fnt");
        label->setScale(0.4f);
        auto btn = CCMenuItemSpriteExtra::create(
            label, this, menu_selector(ModMenuPopup::onTabButton)
        );
        btn->setTag(i);
        btn->setPosition({startX + i * 90.f, y});
        m_tabMenu->addChild(btn);
    }
    m_mainLayer->addChild(m_tabMenu);

    // --- Content container ---
    m_contentLayer = CCNode::create();
    m_contentLayer->setPosition({
        m_mainLayer->getContentSize().width / 2,
        m_mainLayer->getContentSize().height / 2 - 15.f
    });
    m_mainLayer->addChild(m_contentLayer);

    this->selectTab(ModTab::Level);
    return true;
}

void ModMenuPopup::onTabButton(CCObject* sender) {
    auto tag = static_cast<CCNode*>(sender)->getTag();

    // Tags 0-3 are the tab bar. Tags >= 100 are Bot Replay action buttons.
    if (tag < 100) {
        this->selectTab(static_cast<ModTab>(tag));
        return;
    }

    auto br = BotReplay::get();
    switch (tag) {
        case 100: // Record
            br->startRecording();
            Notification::create("Mulai merekam input...", NotificationIcon::Success)->show();
            break;
        case 101: // Stop
            br->stopRecording();
            br->stopPlaying();
            Notification::create("Berhenti.", NotificationIcon::Info)->show();
            break;
        case 102: // Play
            br->startPlaying();
            Notification::create("Memutar ulang replay...", NotificationIcon::Success)->show();
            break;
        case 103: // Reset
            br->reset();
            Notification::create("Replay direset.", NotificationIcon::Info)->show();
            break;
    }

    // Refresh the tab so the status labels update.
    this->selectTab(ModTab::BotReplayTab);
}


void ModMenuPopup::clearContent() {
    m_contentLayer->removeAllChildren();
}

void ModMenuPopup::selectTab(ModTab tab) {
    m_currentTab = tab;
    clearContent();
    switch (tab) {
        case ModTab::Level: buildLevelTab(); break;
        case ModTab::Player: buildPlayerTab(); break;
        case ModTab::Cosmetic: buildCosmeticTab(); break;
        case ModTab::BotReplayTab: buildBotReplayTab(); break;
    }
}

// Placeholder "settings" stored as static bools/floats. Wire these up to
// real gameplay hooks (PlayerObject / GameManager) as needed.
static bool s_noclip = false;
static bool s_hideDeath = false;
static float s_playerScale = 1.0f;

void ModMenuPopup::buildLevelTab() {
    auto info = CCLabelBMFont::create("Pengaturan level saat ini", "bigFont.fnt");
    info->setScale(0.35f);
    info->setPosition({0.f, 70.f});
    m_contentLayer->addChild(info);

    auto toggle = CCMenuItemToggler::createWithStandardSprites(
        this, menu_selector(ModMenuPopup::onTabButton), 0.6f
    );
    // Simple label instead of real toggle wiring for brevity:
    auto noclipLabel = CCLabelBMFont::create("Noclip: OFF (hook PlayerObject::checkCollisions)", "chatFont.fnt");
    noclipLabel->setScale(0.4f);
    noclipLabel->setPosition({0.f, 20.f});
    m_contentLayer->addChild(noclipLabel);
    (void)toggle;
}

void ModMenuPopup::buildPlayerTab() {
    auto info = CCLabelBMFont::create("Player settings (speed, icon, dll)", "bigFont.fnt");
    info->setScale(0.35f);
    info->setPosition({0.f, 40.f});
    m_contentLayer->addChild(info);
}

void ModMenuPopup::buildCosmeticTab() {
    auto info = CCLabelBMFont::create("Cosmetic settings (trail, particle warna, dll)", "bigFont.fnt");
    info->setScale(0.35f);
    info->setPosition({0.f, 40.f});
    m_contentLayer->addChild(info);
}

void ModMenuPopup::buildBotReplayTab() {
    auto br = BotReplay::get();

    auto status = CCLabelBMFont::create(
        fmt::format("Frames tersimpan: {}", br->frameCount()).c_str(),
        "bigFont.fnt"
    );
    status->setScale(0.4f);
    status->setPosition({0.f, 85.f});
    status->setID("br-status-label");
    m_contentLayer->addChild(status);

    auto stateLabel = CCLabelBMFont::create(
        br->isRecording() ? "Status: RECORDING" :
        (br->isPlaying() ? "Status: PLAYING" : "Status: idle"),
        "bigFont.fnt"
    );
    stateLabel->setScale(0.4f);
    stateLabel->setPosition({0.f, 60.f});
    stateLabel->setID("br-state-label");
    m_contentLayer->addChild(stateLabel);

    auto menu = CCMenu::create();
    menu->setPosition({0.f, 0.f});

    auto makeBtn = [&](const char* text, SEL_MenuHandler sel, float x, float y) {
        auto lbl = CCLabelBMFont::create(text, "bigFont.fnt");
        lbl->setScale(0.35f);
        auto btn = CCMenuItemSpriteExtra::create(lbl, this, sel);
        btn->setPosition({x, y});
        menu->addChild(btn);
        return btn;
    };

    makeBtn("Record", menu_selector(ModMenuPopup::onTabButton), -100.f, 20.f)->setTag(100);
    makeBtn("Stop", menu_selector(ModMenuPopup::onTabButton), -35.f, 20.f)->setTag(101);
    makeBtn("Play", menu_selector(ModMenuPopup::onTabButton), 30.f, 20.f)->setTag(102);
    makeBtn("Reset", menu_selector(ModMenuPopup::onTabButton), 95.f, 20.f)->setTag(103);

    m_contentLayer->addChild(menu);

    // Re-route these specific buttons through a small lambda-based handler.
    // Since menu_selector needs a member function, we reuse onTabButton and
    // branch on tag ranges >= 100 for bot replay actions.
}
