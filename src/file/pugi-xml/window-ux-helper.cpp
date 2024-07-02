#include "window-ux-helper.hpp"

#include "exception.hpp"
#include "log-manager.hpp"
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
    Shared<Component> WindowLayoutParseHelper::parseWindowLayoutXml(pugi::xml_node &node, const std::string &log_name) {
        const std::string txt = node.name();
        if (txt == ENABLE_DOCKING_XML) {
            return parseEnableDocking(node);
        } else if (txt == PANEL_XML) {
            return parsePanel(node);
        } else if (txt == TEXT_XML) {
            return parseText(node);
        }
        LogManager manager;
        std::string err_msg = "Invalid XML Format for parsing the window's body layout.";
        if(!manager.hasLog(log_name)) {
            THROW_EXCEPTION(EngineExceptionCode::FILE_EXCEPTION, err_msg);
        }
        std::ostringstream log_output;
        log_output << "Invalid Body XML Tag: " << txt << std::endl;
        manager.writeToLog(log_name, log_output.str());
        THROW_EXCEPTION(EngineExceptionCode::FILE_EXCEPTION, "Body XML Parsing Exception.");
    }
    
    Shared<Component> WindowLayoutParseHelper::parseEnableDocking(pugi::xml_node &node) {
        std::string tag = node.name();
        return Shared<Component>{XmlInterfaceBindingFactory::createComponent(tag)};
    }
    Shared<Component> WindowLayoutParseHelper::parsePanel(pugi::xml_node &node) {
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
            return Shared<Component>{XmlInterfaceBindingFactory::createComponent(tag + "_Bool", name, closing_enabled)};
        } else {
            return Shared<Component>{XmlInterfaceBindingFactory::createComponent(tag, name)};
        }
    }
    Shared<Component> WindowLayoutParseHelper::parseText(pugi::xml_node &node) {
        std::string tag = node.name();
        std::string txt = node.child_value();
        return Shared<Component>{XmlInterfaceBindingFactory::createComponent(tag, txt)};
    }
}