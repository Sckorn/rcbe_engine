#include <type_traits>

#include <X11/keysym.h>

#include <core/EditorInputManager.hpp>
#include <common/utils/stack_utils.hpp>

#include <core/gl_helpers.hpp>
#include <data_types/math/Matrix.hpp>
#include <data_types/math/Vector.hpp>

namespace rcbe::core {
EditorInputManager::EditorInputManager(handler_collection&& h)
{
    register_handlers(std::move(h));
}

}