#pragma once

#include "component.hpp"
#include <memory>
#include "pugixml.hpp"

#include <map>

namespace CzaraEngine {
    class WindowLayoutParseHelper {
        public:
            WindowLayoutParseHelper();
            virtual ~WindowLayoutParseHelper();

            virtual bool isWindowLayoutTag(const std::string &tag);
            virtual std::shared_ptr<Component> parseWindowLayoutXml(pugi::xml_node &node);

        private:
            const static std::string ENABLE_DOCKING_XML;
            const static std::string PANEL_XML;
            const static std::string TEXT_XML;
            const static std::map<std::string, std::string> WINDOW_LAYOUT_TAGS;

        private:
            virtual std::shared_ptr<Component> parseEnableDocking(pugi::xml_node &node);
            virtual std::shared_ptr<Component> parsePanel(pugi::xml_node &node);
            virtual std::shared_ptr<Component> parseText(pugi::xml_node &node);
    };
}