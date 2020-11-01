#include <core/AbstractInputManager.hpp>

#include "EditorInputManager.hpp"
#include "GameInputManager.hpp"

namespace rcbe::core {
AbstractInputManagerPtr create_input_manager(const rendering::RenderingContextPtr& ctx) {
    return EditorInputManager::create(ctx);
}

AbstractInputManagerPtr create_input_manager(nlohmann::json&& j) {
    return GameInputManager::create(std::move(j));
}
}