#include "renderer.hpp"

namespace CzaraEngine {
    Renderer::Renderer(const std::string &dev_name) :
        m_saved_dev_name(dev_name) {}
    Renderer::~Renderer() {}
}