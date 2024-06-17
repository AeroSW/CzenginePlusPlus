#pragma once

#include "xml-callback-binding-factory.hpp"

#ifndef BIND_UX_CALLBACK
    #define BIND_UX_CALLBACK(NAME, FUNC_PTR) \
        namespace { \
            ::CzaraEngine::XmlCallbackBindingFactory::Registrar registrar__##NAME(#NAME, FUNC_PTR); \
        }
#endif