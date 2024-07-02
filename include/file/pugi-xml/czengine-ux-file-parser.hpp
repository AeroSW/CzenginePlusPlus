#pragma once

#include "component.hpp"
#include "file-parser.hpp"
#include "main-menu-helper.hpp"
#include "window-ux-helper.hpp"
#include "pugixml.hpp"

#include <functional>
#include <vector>

typedef std::function<CzaraEngine::Component*(pugi::xml_node&)> ComponentFunction;
typedef std::map<std::string, std::map<std::string, std::string>> CzengineXmlMap;

namespace CzaraEngine {

    /*
        Valid Nodes:
        <XmlFile />
        <StyleFile />
        <Viewport />
        <Window />
        <MainMenuBar />
        <Menu />
        <MenuItem />
        <Separator />
    */
    class CzengineUxFileParser : public FileParser<Shared<Component>> {
        public:
            CzengineUxFileParser(const fs::path &path);
            virtual ~CzengineUxFileParser();
            virtual std::vector<Shared<Component>> processFile();
        private:
            virtual Shared<Component> processPugiNode(pugi::xml_node &node);
            inline std::map<std::string, ComponentFunction>& getNodeParserMap();
            virtual std::string lookUpPugiErr(pugi::xml_parse_status &status);
            MainMenuParseHelper m_main_menu_xml_parse_helper;
            WindowLayoutParseHelper m_window_layout_xml_parse_helper;
    };
}
