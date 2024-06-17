#include "component.hpp"

namespace CzaraEngine {
    Component::Component() : m_render_children(false) {}
    Component::Component(Component * parent) :
        m_render_children(false), m_parent(parent) {}
    Component::Component(const Shared<Component> &parent) :
        m_render_children(false), m_parent(parent) {}
    Component::~Component() {
        children.empty();
    }
    // renderComponent performs a Depth First Recursive Pattern with its children.
    void Component::renderComponent() {
        beginComponent();
        if (m_render_children) {
            for (Shared<Component> &child : children) {
                child.get()->renderComponent();
            }
            endComponent();
        }
    }

    void Component::addParent(Component * component) {
        Shared<Component> parent{component};
        m_parent = parent;
    }

    void Component::addParent(Shared<Component> &component) {
        m_parent = component;
    }

    void Component::addChild(Component * component) {
        Shared<Component> child{component};
        children.push_back(child);
    }

    void Component::addChild(Shared<Component> &component) {
        children.push_back(component);
    }
}