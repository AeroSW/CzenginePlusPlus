#include "dockspace.hpp"
#include "imgui.h"
namespace CzaraEngine {
    DockingEnabler::DockingEnabler() {}
    DockingEnabler::~DockingEnabler() {}
    void DockingEnabler::beginComponent() {
        ImGui::DockSpaceOverViewport();
    }
    void DockingEnabler::endComponent() {}
}