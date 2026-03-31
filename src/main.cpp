#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

class $modify(MyTestMod, MenuLayer) {
    struct Fields {
        async::TaskHolder<web::WebResponse> m_task;
    };

    bool init() {
        if (!MenuLayer::init()) return false;
        
        auto spr = CCSprite::create("button.png"_spr);
        if (!spr) {
            log::error("Failed to load button.png — make sure it's in your resources folder!");
            return true;
        }
        spr->setScale(0.9f);

        auto btn = CCMenuItemSpriteExtra::create(
            spr,
            this,
            menu_selector(MyTestMod::onTestButton)
        );
        btn->setID("geode-test-button");

        auto bottomMenu = this->getChildByID("bottom-menu");
        if (!bottomMenu) {
            log::error("Could not find bottom-menu node!");
            return true;
        }

        bottomMenu->addChild(btn);
        bottomMenu->updateLayout();

        return true;
    }

    void onTestButton(CCObject* sender) {
        web::WebRequest req;
        req.timeout(std::chrono::seconds(10));

        m_fields->m_task.spawn(
            "Fetch useless fact",
            req.get("https://uselessfacts.jsph.pl/api/v2/facts/random?language=en"),
            [](web::WebResponse res) {
                if (!res.ok()) {
                    log::error("Web request failed: {}", res.code());
                    FLAlertLayer::create("Geode Testing", "Failed to fetch a fact :(", "OK")->show();
                    return;
                }

                auto json = res.json().unwrapOr(matjson::Value{});
                std::string source = json.contains("text")
                    ? json["source"].asString().unwrapOr("No text found, check internet")
                    : "No text found.";

                FLAlertLayer::create("Geode Testing", source.c_str(), "OK")->show();
            }
        );
    }
};
