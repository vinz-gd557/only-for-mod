#include "FloatingButton.hpp"
#include "ModMenuPopup.hpp"

FloatingButton* FloatingButton::create() {
    auto ret = new FloatingButton();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool FloatingButton::init() {
    if (!CCNode::init()) return false;

    this->setContentSize({50.f, 50.f});
    this->setAnchorPoint({0.5f, 0.5f});

    auto circle = CCScale9Sprite::create("GJ_button_01.png");
    circle->setContentSize({50.f, 50.f});

    auto label = CCLabelBMFont::create("Mod", "bigFont.fnt");
    label->setScale(0.45f);
    label->setPosition({25.f, 25.f});

    m_btn = CCMenuItemSpriteExtra::create(
        circle, this, menu_selector(FloatingButton::onTap)
    );
    m_btn->setPosition({25.f, 25.f});

    m_menu = CCMenu::create();
    m_menu->setPosition({0.f, 0.f});
    m_menu->addChild(m_btn);
    m_menu->addChild(label);
    this->addChild(m_menu);

    // Default spot: right-middle of the screen.
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    this->setPosition({winSize.width - 40.f, winSize.height / 2});

    // Note: setTouchEnabled/setTouchMode are CCLayer-only convenience
    // methods. Since we extend CCNode directly, we register with the
    // touch dispatcher manually instead.
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1000, true);

    this->scheduleUpdate();
    return true;
}

bool FloatingButton::ccTouchBegan(CCTouch* touch, CCEvent*) {
    auto local = this->convertToNodeSpace(touch->getLocation());
    CCRect box(0, 0, this->getContentSize().width, this->getContentSize().height);
    if (!box.containsPoint(local)) return false;

    m_pressPos = touch->getLocation();
    m_dragging = true;
    m_touchMoved = false;
    return true;
}

void FloatingButton::ccTouchMoved(CCTouch* touch, CCEvent*) {
    if (!m_dragging) return;

    auto loc = touch->getLocation();
    auto delta = loc - m_pressPos;
    if (std::abs(delta.x) > 4.f || std::abs(delta.y) > 4.f) {
        m_touchMoved = true;
    }

    this->setPosition(this->getPosition() + delta);
    m_pressPos = loc;

    // Keep the button on screen.
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto pos = this->getPosition();
    pos.x = std::clamp(pos.x, 25.f, winSize.width - 25.f);
    pos.y = std::clamp(pos.y, 25.f, winSize.height - 25.f);
    this->setPosition(pos);
}

void FloatingButton::ccTouchEnded(CCTouch*, CCEvent*) {
    if (m_dragging && !m_touchMoved) {
        this->onTap(nullptr);
    }
    m_dragging = false;
}

void FloatingButton::onExit() {
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCNode::onExit();
}

void FloatingButton::onTap(CCObject*) {
    ModMenuPopup::create()->show();
}
