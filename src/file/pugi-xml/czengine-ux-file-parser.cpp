#include "czengine-ux-file-parser.hpp"
#include "exception.hpp"
#include "xml-interface-binding-factory.hpp"
#include "xml-callback-binding-factory.hpp"
#include <sstream>
#include <iostream>

namespace CzaraEngine {

    CzengineUxFileParser::CzengineUxFileParser(const fs::path &path) :
        FileParser<std::shared_ptr<Component>>(path), m_main_menu_xml_parse_helper(),
        m_window_layout_xml_parse_helper() {}
    CzengineUxFileParser::~CzengineUxFileParser() {}
    std::vector<std::shared_ptr<Component>> CzengineUxFileParser::processFile() {
        pugi::xml_document document;
        pugi::xml_parse_result result = document.load_file(m_path.c_str());
        if (result.status == pugi::xml_parse_status::status_ok) {
            std::vector<std::shared_ptr<Component>> ux_components;
            pugi::xml_node content_node = document.first_child();
            std::string content_tag = content_node.name();
            if (content_tag != "Content") {
                std::ostringstream msg;
                msg << "XML Document Formatting Failure\n";
                msg << "\tFirst XML Tag in " << m_path << " is not '<Content>'." << std::endl;
                getLogManager().writeToLog(m_log_name, msg.str());
                THROW_EXCEPTION(EngineExceptionCode::FILE_EXCEPTION, "Invalid XML format.");
            }
            for (pugi::xml_node child = content_node.first_child(); child; child = child.next_sibling()) {
                std::string tag = child.name();
                if (tag == "File") {
                    std::string name = child.attribute("name").value();
                    fs::path file_path{name};
                    if (file_path == this->m_path) {
                        continue; // Skip if same file path as current.
                    }
                    CzengineUxFileParser nested_parser{file_path};
                    std::vector<std::shared_ptr<Component>> parsed_components = nested_parser.processFile();
                    ux_components.insert(std::end(ux_components), std::begin(parsed_components), std::end(parsed_components));
                } else {
                    std::shared_ptr<Component> component = processPugiNode(child);
                    ux_components.push_back(component);
                }
            }
            return ux_components;
        } else {
            std::ostringstream msg;
            msg << "XML Document Load Failure\n";
            msg << "\tPugiXML Result Status:     \t" << lookUpPugiErr(result.status) << "\n";
            msg << "\tPugiXML Result Description:\t" << result.description() << "\n";
            msg << "\tPugiXML File:              \t" << m_path << std::endl;
            std::string log_name = m_log_name;
            getLogManager().writeToLog(log_name, msg.str());
            std::ostringstream exception_msg;
            exception_msg << "Failed to load Ux Specification.  Please check '" << m_log_name << "' logs for details.";
            THROW_EXCEPTION(EngineExceptionCode::FILE_EXCEPTION, exception_msg.str());
        }
    }
    std::shared_ptr<Component> CzengineUxFileParser::processPugiNode(pugi::xml_node &node) {
        // TODO: Figure out Factory Key based on XML Tag, Value, and Attributes
        std::string tag = node.name();
        std::shared_ptr<Component> component;
        if (m_main_menu_xml_parse_helper.isMainMenuTag(tag)) {
            component = m_main_menu_xml_parse_helper.parseMainMenuXml(node, m_log_name);
        } else if (m_window_layout_xml_parse_helper.isWindowLayoutTag(tag)) {
            component = m_window_layout_xml_parse_helper.parseWindowLayoutXml(node, m_log_name);
        }
        if (component) {
            for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
                std::string child_tag = child.name();
                // If tag size is 0, then it isn't an <xml> tag
                if (child_tag.size() == 0) continue;
                std::shared_ptr<Component> child_component = processPugiNode(child);
                component.get()->addChild(child_component);
                
                child_component.get()->addParent(component);
            }
            return component;
        }
        std::ostringstream exception_msg;
        exception_msg << "Unable to determine tag type (" << tag << ").";
        std::cerr << exception_msg.str() << std::endl;
        THROW_EXCEPTION(EngineExceptionCode::FILE_EXCEPTION, exception_msg.str());
    }
    std::string CzengineUxFileParser::lookUpPugiErr(pugi::xml_parse_status &status) {
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
}