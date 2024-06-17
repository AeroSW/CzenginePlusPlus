#pragma once

#include <functional>
#include <map>
#include <string>

namespace {
    class XmlCallbackBindingFactory {
        public:
            class Registrar {
                public:
                    Registrar(const std::string&, std::function<void()> &func);
                    virtual ~Registrar();
                private:
                    std::string m_key;
            };
            static inline std::function<void()>& retrieveFunction(const std::string &key) {
                auto it = getMap().find(key);
                if (it == getMap().end()) {
                    static std::function<void()> null_function = nullptr;
                    return null_function;
                };
                return it->second;
            }
        private:
            static std::map<std::string, std::function<void()>>& getMap() {
                static std::map<std::string, std::function<void()>> m_map;
                return m_map;
            }

    };
    XmlCallbackBindingFactory::Registrar::Registrar(const std::string &key, std::function<void()> &func) :
        m_key(key) {
        getMap()[key] = func;
    }
    XmlCallbackBindingFactory::Registrar::~Registrar() {
        auto it = getMap().find(m_key); // Find in case this Registrar's key still exists
        if(it != getMap().end()){ 	// Check if it does exist
            getMap().erase(it); 	// Erase it from the mapping. Cleanup is important!
        }
    }
}
