#include <type_traits>

#ifdef __linux__
#include <X11/keysym.h>
#endif

#include <rcbe-engine/core/EditorInputManager.hpp>
#ifdef RDMN_OPENGL
#include <rcbe-engine/core/gl_helpers.hpp>
#endif
#include <rcbe-engine/datamodel/math/Matrix.hpp>
#include <rcbe-engine/datamodel/math/Vector.hpp>

namespace rcbe::core {
EditorInputManager::EditorInputManager(HandlerCollection &&h) {
    registerHandlers(std::move(h));
}

}// namespace rcbe::core