#include "main-menu-helper.hpp"

#include "exception.hpp"
#include "app-logs.hpp"
#include "xml-interface-binding-factory.hpp"
#include "xml-callback-binding-factory.hpp"

namespace CzaraEngine {
    const std::string MainMenuParseHelper::MAIN_MENU_BAR_XML = "MainMenuBar";
    const std::string MainMenuParseHelper::MENU_XML = "Menu";
    const std::string MainMenuParseHelper::MENU_ITEM_XML = "MenuItem";
    const std::string MainMenuParseHelper::MENU_SEPARATOR_XML = "Separator";
    const std::map<std::string, std::string> MainMenuParseHelper::MAIN_MENU_TAGS = {
        {MAIN_MENU_BAR_XML, MAIN_MENU_BAR_XML},
        {MENU_XML, MENU_XML},
        {MENU_ITEM_XML, MENU_ITEM_XML},
        {MENU_SEPARATOR_XML, MENU_SEPARATOR_XML}
    };
    MainMenuParseHelper::MainMenuParseHelper() {}
    MainMenuParseHelper::~MainMenuParseHelper() {}
    bool MainMenuParseHelper::isMainMenuTag(const std::string &tag) {
        return MAIN_MENU_TAGS.find(tag) != MAIN_MENU_TAGS.end();
    }
    std::shared_ptr<Component> MainMenuParseHelper::parseMainMenuXml(pugi::xml_node &node) {
        const std::string text = node.name();
        if (text == MAIN_MENU_BAR_XML) {
            return parseMainMenuBar(node);
        } else if (text == MENU_XML) {
            return parseMenu(node);
        } else if (text == MENU_ITEM_XML) {
            return parseMenuItem(node);
        } else if (text == MENU_SEPARATOR_XML) {
            return parseMenuSeparator(node);
        }
        Logger::file_log() << "Invalid Main Menu XML Tag: " << text << endl;
        THROW_EXCEPTION(EngineExceptionCode::FILE_EXCEPTION, "Main Menu XML Parsing Exception.");
    }
    std::shared_ptr<Component> MainMenuParseHelper::parseMainMenuBar(pugi::xml_node &node) {
        std::string tag = node.name();
        return std::shared_ptr<Component>{XmlInterfaceBindingFactory::createComponent(tag)};
	}
    std::shared_ptr<Component> MainMenuParseHelper::parseMenu(pugi::xml_node &node) {
        std::string tag = node.name();
        std::string value = node.attribute("name").value();
        return std::shared_ptr<Component>{XmlInterfaceBindingFactory::createComponent(tag, value)};
	}
    std::shared_ptr<Component> MainMenuParseHelper::parseMenuItem(pugi::xml_node &node) {
        std::string tag = node.name();
        std::string value = node.attribute("name").value();
        std::string shortcut = node.attribute("shortcut").value();
        std::string callback_name = node.attribute("callback").value();
        std::function<void()> callback = nullptr;
        if (callback_name.size() > 0) {
            callback = XmlCallbackBindingFactory::retrieveFunction(callback_name);
        }
        if (callback != nullptr && shortcut.size() > 0) {
            std::string final_name = tag + "_Shortcut_Callback";
            return std::shared_ptr<Component>{XmlInterfaceBindingFactory::createComponent<std::string, std::string, std::function<void()>>(final_name, value, shortcut, callback)};
        } else if (callback != nullptr) {
            std::string final_name = tag + "_Callback";
            return std::shared_ptr<Component>{XmlInterfaceBindingFactory::createComponent<std::string, std::function<void()>>(final_name, value, callback)};
        } else if (shortcut.size() > 0) {
            std::string final_name = tag + "_Shortcut";
            return std::shared_ptr<Component>{XmlInterfaceBindingFactory::createComponent<std::string, std::string>(final_name, value, shortcut)};
        } else {
            return std::shared_ptr<Component>{XmlInterfaceBindingFactory::createComponent<std::string>(tag, value)};
        }
	}
    std::shared_ptr<Component> MainMenuParseHelper::parseMenuSeparator(pugi::xml_node &node) {
        std::string tag = node.name();
        std::string value = node.attribute("name").value();
        if (!value.empty()) {
            return std::shared_ptr<Component>{XmlInterfaceBindingFactory::createComponent(tag + "_Text", value)};
        } else {
            return std::shared_ptr<Component>{XmlInterfaceBindingFactory::createComponent(tag)};
        }
	}

}