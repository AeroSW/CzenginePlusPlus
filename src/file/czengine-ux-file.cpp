#include "czengine-ux-file.hpp"
#include "exception.hpp"
#include "xml-interface-binding-factory.hpp"
#include "xml-callback-binding-factory.hpp"
#include <sstream>

namespace CzaraEngine {
    CzengineUxFile::CzengineUxFile(const fs::path &path) :
        CzengineFile<Shared<Component>>(path) {}
    CzengineUxFile::~CzengineUxFile() {}
    std::vector<Shared<Component>> CzengineUxFile::processFile() {
        pugi::xml_document document;
        pugi::xml_parse_result result = document.load_file(m_path.c_str());
        if (result.status == pugi::xml_parse_status::status_ok) {
            std::vector<Shared<Component>> ux_components;
            for (pugi::xml_node child = document.first_child(); child; child = child.next_sibling()) {
                Shared<Component> component = processPugiNode(child);
                ux_components.push_back(component);
            }
            return ux_components;
        } else {
            std::ostringstream msg;
            msg << "XML Document Load Failure\n";
            msg << "\tPugiXML Result Status:     \t" << lookUpPugiErr(result.status) << "\n";
            msg << "\tPugiXML Result Description:\t" << result.description() << "\n";
            std::string log_name = m_log_name;
            getLogManager().writeToLog(log_name, msg.str());
            std::ostringstream exception_msg;
            exception_msg << "Failed to load Ux Specification.  Please check '" << m_log_name << "' logs for details.";
            THROW_EXCEPTION(EngineExceptionCode::FILE_EXCEPTION, exception_msg.str());
        }
    }
    Shared<Component> CzengineUxFile::processPugiNode(pugi::xml_node &node) {
        // TODO: Figure out Factory Key based on XML Tag, Value, and Attributes
        Shared<Component> component{ determineBaseTag(node) };
        for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
            Shared<Component> child_component = processPugiNode(child);
            component.get()->addChild(child_component);
            
            child_component.get()->addParent(component);
        }
        return component;
    }
    std::string CzengineUxFile::lookUpPugiErr(pugi::xml_parse_status &status) {
        return (std::string[]) {
            "status_ok",
            "status_file_not_found",
            "status_io_error",
            "status_out_of_memory",
            "status_internal_error",
            "status_unrecognized_tag",
            "status_bad_pi",
            "status_bad_comment",
            "status_bad_cdata",
            "status_bad_doctype",
            "status_bad_pcdata",
            "status_bad_start_element",
            "status_bad_attribute",
            "status_bad_end_element",
            "status_end_element_mismatch",
            "status_append_invalid_root",
            "status_no_document_element"
        }[status];
    }
    using compFunc = std::function<Component*(pugi::xml_node&)>;
    Component* CzengineUxFile::determineBaseTag(pugi::xml_node &node) {
        static std::map<std::string, compFunc> tag_map = {
            {"MainMenuBar", [](pugi::xml_node &node) -> Component* {return XmlInterfaceBindingFactory::createComponent(node.name());}},
            {"Menu", [](pugi::xml_node &node) -> Component* {
                std::string name = node.attribute("name").value();
                return XmlInterfaceBindingFactory::createComponent(node.name(), name);
            }},
            {"MenuItem", std::bind(CzengineUxFile::handleMenuItem, this, std::placeholders::_1)},
            {"Separator", std::bind(CzengineUxFile::handleSeparator, this, std::placeholders::_1)}
        };
        std::string tag = node.name();
        return tag_map[tag](node);
    }
    Component* CzengineUxFile::handleMenuItem(pugi::xml_node &node) {
        std::map<std::string, std::string> node_attr_map;
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
            return XmlInterfaceBindingFactory::createComponent<std::string, std::string, std::function<void()>>(final_name, value, shortcut, callback);
        } else if (callback != nullptr) {
            std::string final_name = tag + "_Callback";
            return XmlInterfaceBindingFactory::createComponent<std::string, std::function<void()>>(final_name, value, callback);
        } else if (shortcut.size() > 0) {
            std::string final_name = tag + "_Shortcut";
            return XmlInterfaceBindingFactory::createComponent<std::string, std::string>(final_name, value, shortcut);
        } else {
            return XmlInterfaceBindingFactory::createComponent<std::string>(tag, value);
        }
        
    }
    Component* CzengineUxFile::handleSeparator(pugi::xml_node &node) {
        std::string tag = node.name();
        std::string value = node.attribute("name").value();
        if (!value.empty()) {
            return XmlInterfaceBindingFactory::createComponent(tag + "_Text", value);
        } else {
            return XmlInterfaceBindingFactory::createComponent(tag);
        }
    }
}