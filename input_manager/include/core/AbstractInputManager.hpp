#ifndef RCBE_ENGINE_ABSTRACTINPUTMANAGER_HPP
#define RCBE_ENGINE_ABSTRACTINPUTMANAGER_HPP

#include <memory>
#include <variant>

#include <boost/log/trivial.hpp>

#include <rcbe-engine/core/EditorInputManager.hpp>
#include <rcbe-engine/core/GameInputManager.hpp>

#include <rcbe-engine/traits/input_manager.hpp>

namespace rcbe::core {
using InputManagerVariant = std::variant<EditorInputManager, GameInputManager>;

class AbstractInputManager {
public:

    explicit AbstractInputManager(EditorInputManager&& manager)
    :
    variant_ (std::move(manager))
    {

    }

    explicit AbstractInputManager(GameInputManager&& manager)
    :
    variant_ (std::move(manager))
    {

    }

    decltype(auto) tryProcessEvent(XEvent& event) {
        return std::visit([&event](auto& arg) mutable -> bool {
            auto ret = arg.tryProcessEvent(event);
            return ret;
        }, variant_);
    }

#ifdef RCBE_DEBUG_MODE
    //*For the simplicity of debug*/
    InputManagerVariant& getVariant() {
        return variant_;
    }
#endif

private:
    InputManagerVariant variant_;
};

using AbstractInputManagerPtr = std::shared_ptr<AbstractInputManager>;
using AbstractInputManagerConstPtr = std::shared_ptr<const AbstractInputManager>;
}

#endif //RCBE_ENGINE_ABSTRACTINPUTMANAGER_HPP
