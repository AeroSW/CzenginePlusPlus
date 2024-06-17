#pragma once

#include "component.hpp"
#include "czengine-file.hpp"
#include "pugixml.hpp"

namespace CzaraEngine {
    class CzengineUxFile : public CzengineFile<Shared<Component>> {
        public:
            CzengineUxFile(const fs::path &path);
            virtual ~CzengineUxFile();
            virtual std::vector<Shared<Component>> processFile();
        private:
            virtual Shared<Component> processPugiNode(pugi::xml_node &node);
            virtual Component* determineBaseTag(pugi::xml_node &node);
            virtual Component* handleMenuItem(pugi::xml_node &node);
            virtual Component* handleSeparator(pugi::xml_node &node);
            virtual std::string lookUpPugiErr(pugi::xml_parse_status &status);
    };
}
