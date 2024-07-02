#pragma once

#include "inttypes.hpp"
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

namespace CzaraEngine {
    enum XmlStatus {
        SUCCESS,
        FAILURE,
        NA
    };
    class XmlNode {
        public:
            virtual std::string getElement() = 0;
            virtual std::string getValue() = 0;
            virtual std::string getAttribute(const std::string &name) = 0;
            virtual i32 getAttributeAsInt(const std::string &name) = 0;
            virtual float getAttributeAsFloat(const std::string &name) = 0;
            virtual XmlNode getChild() = 0;
            virtual XmlNode getSibling() = 0;
            virtual bool hasNextSibling() = 0;
    };
    class XmlDocument {
        public:
            virtual XmlStatus openDocument(fs::path &path) = 0;
            virtual XmlNode getChild() = 0;
            virtual XmlStatus getStatus() = 0;
    };
}