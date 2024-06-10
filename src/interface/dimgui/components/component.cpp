#include "component.hpp"

namespace CzaraEngine {
    Component::Component() : renderChildren(false) {}
    Component::~Component() {
        children.empty();
    }
    // renderComponent performs a Depth First Recursive Pattern with its children.
    void Component::renderComponent() {
        beginComponent();
        if (renderChildren) {
            for (Shared<Component> &child : children) {
                child.get()->renderComponent();
            }
        }
        endComponent();
    }

    void Component::addChild(Component * component) {
        Shared<Component> child{component};
        children.push_back(child);
    }

    void Component::addChild(Shared<Component> &component) {
        children.push_back(component);
    }
}