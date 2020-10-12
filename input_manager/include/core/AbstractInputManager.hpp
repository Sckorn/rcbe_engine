#ifndef RCBE_ENGINE_ABSTRACTINPUTMANAGER_HPP
#define RCBE_ENGINE_ABSTRACTINPUTMANAGER_HPP

#include <memory>
#include <variant>

#include <boost/log/trivial.hpp>

#include <core/EditorInputManager.hpp>
#include <core/GameInputManager.hpp>

#include <traits/input_manager.hpp>

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

    decltype(auto) try_process_event(XEvent& event) {
        return std::visit([&event](auto& arg) mutable -> bool {
            auto ret = arg.try_process_event(event);
            return ret;
        }, variant_);
    }

private:
    InputManagerVariant variant_;
};

using AbstractInputManagerPtr = std::unique_ptr<AbstractInputManager>;
using AbstractInputManagerConstPtr = std::unique_ptr<const AbstractInputManager>;
}

#endif //RCBE_ENGINE_ABSTRACTINPUTMANAGER_HPP
