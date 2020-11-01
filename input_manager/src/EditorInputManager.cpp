#include <type_traits>

#include <core/EditorInputManager.hpp>

namespace rcbe::core {

EditorInputManager::EditorInputManager(handler_collection&& h) {
    register_handlers(std::move(h));
}

}