#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

// A small circular button that floats on top of every scene, can be
// dragged anywhere on screen, and opens the mod menu popup on tap
// (tap = press+release without significant movement; drag = press+move).
// Inherits CCTouchDelegate explicitly since plain CCNode has no touch
// interface of its own (that lives on CCTouchDelegate / CCLayer).
class FloatingButton : public CCNode, public cocos2d::CCTouchDelegate {
protected:
    CCMenu* m_menu = nullptr;
    CCMenuItemSpriteExtra* m_btn = nullptr;
    CCPoint m_pressPos;
    bool m_dragging = false;
    bool m_touchMoved = false;

    bool init();
    void onTap(CCObject*);

public:
    static FloatingButton* create();

    bool ccTouchBegan(CCTouch* touch, CCEvent* event) override;
    void ccTouchMoved(CCTouch* touch, CCEvent* event) override;
    void ccTouchEnded(CCTouch* touch, CCEvent* event) override;
    void onExit() override;
};
