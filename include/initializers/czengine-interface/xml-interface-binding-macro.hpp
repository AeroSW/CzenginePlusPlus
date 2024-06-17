#pragma once

#include "xml-interface-binding-factory.hpp"

#ifndef BIND_UX_COMPONENT
    #define BIND_UX_COMPONENT(NAME, TYPE, ...) \
        namespace { \
            ::CzaraEngine::XmlInterfaceBindingFactory::Registrar<TYPE, ##__VA_ARGS__> registrar__##NAME(#NAME); \
        }
#endif
