#include "window-ux-helper.hpp"

#include "exception.hpp"
#include "app-logs.hpp"
#include "dimgui-window.hpp"
#include "xml-interface-binding-factory.hpp"

namespace CzaraEngine {
    const std::string WindowLayoutParseHelper::ENABLE_DOCKING_XML = "EnableDocking";
    const std::string WindowLayoutParseHelper::PANEL_XML = "Panel";
    const std::string WindowLayoutParseHelper::TEXT_XML = "Text";
    const std::map<std::string, std::string> WindowLayoutParseHelper::WINDOW_LAYOUT_TAGS = {
        {ENABLE_DOCKING_XML, ENABLE_DOCKING_XML},
        {PANEL_XML, PANEL_XML},
        {TEXT_XML, TEXT_XML}
    };
    WindowLayoutParseHelper::WindowLayoutParseHelper() {}
    WindowLayoutParseHelper::~WindowLayoutParseHelper() {}

    bool WindowLayoutParseHelper::isWindowLayoutTag(const std::string &tag) {
        return WINDOW_LAYOUT_TAGS.find(tag) != WINDOW_LAYOUT_TAGS.end();
    }
    std::shared_ptr<Component> WindowLayoutParseHelper::parseWindowLayoutXml(pugi::xml_node &node) {
        const std::string txt = node.name();
        if (txt == ENABLE_DOCKING_XML) {
            return parseEnableDocking(node);
        } else if (txt == PANEL_XML) {
            return parsePanel(node);
        } else if (txt == TEXT_XML) {
            return parseText(node);
        }
        Logger::file_log() << "Invalid Body XML Tag: " << txt << endl;
        THROW_EXCEPTION(EngineExceptionCode::FILE_EXCEPTION, "Body XML Parsing Exception.");
    }
    
    std::shared_ptr<Component> WindowLayoutParseHelper::parseEnableDocking(pugi::xml_node &node) {
        std::string tag = node.name();
        return std::shared_ptr<Component>{XmlInterfaceBindingFactory::createComponent(tag)};
    }
    std::shared_ptr<Component> WindowLayoutParseHelper::parsePanel(pugi::xml_node &node) {
        std::string tag = node.name();
        std::string name = node.attribute("name").value();
        std::string close_flag_attr = node.attribute("enable_closing").name();
        bool closing_enabled = false;
        if (close_flag_attr.size() != 0) {
            closing_enabled = node.attribute("enable_closing").as_bool(true);
        }
        PanelProps props = { name };
        // TODO: Enable users to provide flags in XML and parse them as part of "props".
        if (closing_enabled) {
            return std::shared_ptr<Component>{XmlInterfaceBindingFactory::createComponent(tag + "_Bool", props, closing_enabled)};
        } else {
            return std::shared_ptr<Component>{XmlInterfaceBindingFactory::createComponent(tag, props)};
        }
    }
    std::shared_ptr<Component> WindowLayoutParseHelper::parseText(pugi::xml_node &node) {
        std::string tag = node.name();
        std::string txt = node.child_value();
        return std::shared_ptr<Component>{XmlInterfaceBindingFactory::createComponent(tag, txt)};
    }
}