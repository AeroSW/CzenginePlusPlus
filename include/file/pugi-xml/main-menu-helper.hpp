#pragma once

#include "component.hpp"
#include <memory>
#include "pugixml.hpp"

#include <map>

namespace CzaraEngine {
    class MainMenuParseHelper {
        public:
            MainMenuParseHelper();
            virtual ~MainMenuParseHelper();
            virtual bool isMainMenuTag(const std::string &tag);
            virtual std::shared_ptr<Component> parseMainMenuXml(pugi::xml_node &node, const std::string &log_name);
        private:
            const static std::string MAIN_MENU_BAR_XML;
            const static std::string MENU_XML;
            const static std::string MENU_ITEM_XML;
            const static std::string MENU_SEPARATOR_XML;
            const static std::map<std::string, std::string> MAIN_MENU_TAGS;
        private:
            virtual std::shared_ptr<Component> parseMainMenuBar(pugi::xml_node &node);
            virtual std::shared_ptr<Component> parseMenu(pugi::xml_node &node);
            virtual std::shared_ptr<Component> parseMenuItem(pugi::xml_node &node);
            virtual std::shared_ptr<Component> parseMenuSeparator(pugi::xml_node &node);
    };
}