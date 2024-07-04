#include "component.hpp"

namespace CzaraEngine {
    Component::Component() : m_render_children(false) {}
    Component::Component(Component * parent) :
        m_render_children(false), m_parent(parent) {}
    Component::Component(const std::shared_ptr<Component> &parent) :
        m_render_children(false), m_parent(parent) {}
    Component::~Component() {
        children.empty();
    }
    // renderComponent performs a Depth First Recursive Pattern with its children.
    void Component::renderComponent() {
        beginComponent();
        if (m_render_children) {
            for (std::shared_ptr<Component> &child : children) {
                child.get()->renderComponent();
            }
            endComponent();
        }
    }

    void Component::addParent(Component * component) {
        std::shared_ptr<Component> parent{component};
        m_parent = parent;
    }

    void Component::addParent(std::shared_ptr<Component> &component) {
        m_parent = component;
    }

    void Component::addChild(Component * component) {
        std::shared_ptr<Component> child{component};
        children.push_back(child);
    }

    void Component::addChild(std::shared_ptr<Component> &component) {
        children.push_back(component);
    }
}