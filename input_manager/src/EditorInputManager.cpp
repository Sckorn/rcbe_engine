#include <type_traits>

#include <X11/keysym.h>

#include <rcbe-engine/core/EditorInputManager.hpp>
#include <rcbe-engine/core/gl_helpers.hpp>
#include <rcbe-engine/datamodel/math/Matrix.hpp>
#include <rcbe-engine/datamodel/math/Vector.hpp>

namespace rcbe::core {
EditorInputManager::EditorInputManager(HandlerCollection &&h) {
    registerHandlers(std::move(h));
}

}// namespace rcbe::core