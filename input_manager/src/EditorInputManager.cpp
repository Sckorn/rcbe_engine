#include <type_traits>

#include <X11/keysym.h>

#include <core/EditorInputManager.hpp>

#include <core/gl_helpers.hpp>
#include <rcbe-engine/datamodel/math/Matrix.hpp>
#include <rcbe-engine/datamodel/math/Vector.hpp>

namespace rcbe::core {
//TODO: looks useless, consider relocating to header
EditorInputManager::EditorInputManager(handler_collection&& h)
{
    register_handlers(std::move(h));
}

}