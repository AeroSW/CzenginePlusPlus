#pragma once

#include <map>
#include <string>
#include <functional>
#include "component.hpp"

namespace CzaraEngine {
    /** In depth documentation describing this structure can be found at https://github.com/AeroSW/XmlInterfaceBindingFactory.git */
    class XmlInterfaceBindingFactory {
        public:
            template<typename U, typename ...A>
            class Registrar {
                public:
                    Registrar(const std::string &key);
                    virtual ~Registrar();
                private:
                    std::string m_key;
            };
            template<typename ...A>
            static inline Component * createComponent(const std::string &key, A... args) {
                auto it = getMap().find(key);
                if (it == getMap().end()) return nullptr;
                std::size_t num_args = sizeof...(A);
                if (num_args != it->second.first) return nullptr;
                DerivedFunctor<Component*, A...> * df = static_cast<DerivedFunctor<Component*, A...>*>(it->second.second);
                return (*df)(args...);
            }
        private:
            class Functor {
                public:
                    virtual ~Functor() {}
            };
            typedef std::pair<std::size_t, CzaraEngine::XmlInterfaceBindingFactory::Functor*> SizedFunctorPair;
            template<typename R, typename ...A>
            class DerivedFunctor : public Functor {
                public:
                    DerivedFunctor(std::function<R(A...)> functor);
                    DerivedFunctor(const DerivedFunctor<R, A...> &df);
                    virtual ~DerivedFunctor();
                    virtual R operator()(A... args);
                    virtual DerivedFunctor<R, A...>& operator=(const DerivedFunctor<R, A...> &df);
                private:
                    std::function<R(A...)> m_functor;
            };
            static std::map<std::string, SizedFunctorPair>& getMap() {
                static std::map<std::string, SizedFunctorPair> m_map;
                return m_map;
            }
            template<typename U, typename... A>
            static Component * create(A... args);
    };

    template<typename R, typename... A> // Basic Constructor.
    XmlInterfaceBindingFactory::DerivedFunctor<R,A...>::DerivedFunctor(std::function<R(A...)> functor) :
        m_functor(functor) {}
    template<typename R, typename... A> // Copy Constructor.
    XmlInterfaceBindingFactory::DerivedFunctor<R,A...>::DerivedFunctor(const XmlInterfaceBindingFactory::DerivedFunctor<R,A...>& df) :
        m_functor(df.m_functor) {}
    template<typename R, typename... A> // Destructor
    XmlInterfaceBindingFactory::DerivedFunctor<R,A...>::~DerivedFunctor(){}

    // Overloaded Operators for Derived Functor
    template<typename R, typename... A> // Overloaded () operator
    R XmlInterfaceBindingFactory::DerivedFunctor<R,A...>::operator()(A... args){
        return m_functor(args...);
    }
    template<typename R, typename... A> // Overloaded = operator
    XmlInterfaceBindingFactory::DerivedFunctor<R,A...>& XmlInterfaceBindingFactory::DerivedFunctor<R,A...>::operator=(const XmlInterfaceBindingFactory::DerivedFunctor<R,A...>& df){
        m_functor = df.m_functor;
        return *this;
    }

    template<typename U, typename... A>
    Component * XmlInterfaceBindingFactory::create(A... args){
        return new U(args...);
    }

    template<typename U, typename... A>
    XmlInterfaceBindingFactory::Registrar<U,A...>::Registrar(const std::string &key){
        DerivedFunctor<Component*,A...> * df = new DerivedFunctor<Component*,A...>(XmlInterfaceBindingFactory::create<U,A...>);
        const std::size_t args_size = sizeof...(A);
        getMap()[key] = std::make_pair(args_size, df);
        m_key = key;
    }
    template<typename U, typename... A>
    XmlInterfaceBindingFactory::Registrar<U,A...>::~Registrar(){
        auto it = getMap().find(m_key); // Find in case this Registrar's key still exists
        if(it != getMap().end()){ 	// Check if it does exist
            delete it->second.second; 		// If so, delete the Functor
            getMap().erase(it); 	// Erase it from the mapping. Cleanup is important!
        }
    }
}
