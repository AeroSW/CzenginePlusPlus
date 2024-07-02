#pragma once

#include "component.hpp"
#include "shared.hpp"
#include "pugixml.hpp"

#include <map>

namespace CzaraEngine {
    class MainMenuParseHelper {
        public:
            MainMenuParseHelper();
            virtual ~MainMenuParseHelper();
            virtual bool isMainMenuTag(const std::string &tag);
            virtual Shared<Component> parseMainMenuXml(pugi::xml_node &node, const std::string &log_name);
        private:
            const static std::string MAIN_MENU_BAR_XML;
            const static std::string MENU_XML;
            const static std::string MENU_ITEM_XML;
            const static std::string MENU_SEPARATOR_XML;
            const static std::map<std::string, std::string> MAIN_MENU_TAGS;
        private:
            virtual Shared<Component> parseMainMenuBar(pugi::xml_node &node);
            virtual Shared<Component> parseMenu(pugi::xml_node &node);
            virtual Shared<Component> parseMenuItem(pugi::xml_node &node);
            virtual Shared<Component> parseMenuSeparator(pugi::xml_node &node);
    };
}