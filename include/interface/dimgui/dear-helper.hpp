#pragma once
namespace CzaraEngine {
    class DearImGuiInterface;
    class DearHelper {
        public:
            static bool sdlGuard(DearImGuiInterface &interface);
            static bool vulkanGuard(DearImGuiInterface &interface);
    };
}
