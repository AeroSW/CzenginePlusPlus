#pragma once

#include "shared.hpp"
#include <vector>

namespace CzaraEngine {
    
    // Component is base virtual class that imitates tree structure.
    class Component {
        public:
            virtual ~Component();
            // renderComponent performs a Depth First Recursive Pattern with its children.
            virtual void renderComponent();
            virtual void addParent(Component * component);
            virtual void addParent(Shared<Component> &component);
            virtual void addChild(Component * component);
            virtual void addChild(Shared<Component> &component);
        protected:
            Component();
            Component(Component * parent);
            Component(const Shared<Component> &parent);
            // Method to execute any code prior to recursive call.
            // Able to set `renderChildren` to falsy to avoid rendering
            // child componens.
            virtual void beginComponent() = 0;
            // Method to execute any code post recursive call.
            virtual void endComponent() = 0;

            bool m_render_children = false;
            Shared<Component> m_parent;
            std::vector<Shared<Component>> children;
    };
}
