#pragma once

#include "component.hpp"
#include "shared.hpp"
#include "pugixml.hpp"

#include <map>

namespace CzaraEngine {
    class WindowLayoutParseHelper {
        public:
            WindowLayoutParseHelper();
            virtual ~WindowLayoutParseHelper();

            virtual bool isWindowLayoutTag(const std::string &tag);
            virtual Shared<Component> parseWindowLayoutXml(pugi::xml_node &node, const std::string &log_name);

        private:
            const static std::string ENABLE_DOCKING_XML;
            const static std::string PANEL_XML;
            const static std::string TEXT_XML;
            const static std::map<std::string, std::string> WINDOW_LAYOUT_TAGS;

        private:
            virtual Shared<Component> parseEnableDocking(pugi::xml_node &node);
            virtual Shared<Component> parsePanel(pugi::xml_node &node);
            virtual Shared<Component> parseText(pugi::xml_node &node);
    };
}