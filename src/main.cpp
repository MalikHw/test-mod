#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

class $modify(MyTestMod, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        // Load custom sprite from mod resources
        auto spr = CCSprite::create("button.png"_spr);
        if (!spr) {
            log::error("Failed to load button.png — make sure it's in your resources folder!");
            return true;
        }

        // Scale it to fit nicely alongside other bottom-menu buttons
        spr->setScale(0.9f);

        // Create the button
        auto btn = CCMenuItemSpriteExtra::create(
            spr,
            this,
            menu_selector(MyTestMod::onTestButton)
        );
        btn->setID("geode-test-button");

        // Add to the existing bottom-menu
        auto bottomMenu = this->getChildByID("bottom-menu");
        if (!bottomMenu) {
            log::error("Could not find bottom-menu node!");
            return true;
        }

        bottomMenu->addChild(btn);
        bottomMenu->updateLayout(); // reflow the layout to include the new button

        return true;
    }

    void onTestButton(CCObject* sender) {
        FLAlertLayer::create("Geode Testing", "geode testing", "OK")->show();
    }
};
