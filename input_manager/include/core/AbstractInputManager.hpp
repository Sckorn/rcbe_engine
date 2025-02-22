#ifndef RCBE_ENGINE_ABSTRACTINPUTMANAGER_HPP
#define RCBE_ENGINE_ABSTRACTINPUTMANAGER_HPP

#include <memory>
#include <variant>

#include <rcbe-engine/core/InputManagerTraits.hpp>
#include <rcbe-engine/core/EditorInputManager.hpp>
#include <rcbe-engine/core/GameInputManager.hpp>
#include <rcbe-engine/datamodel/system/input_system_types.hpp>

namespace rcbe::core {
using InputManagerVariant = std::variant<EditorInputManager, GameInputManager>;

class R_PUBLIC_API AbstractInputManager {
public:

    explicit AbstractInputManager(EditorInputManager &&manager)
        : variant_(std::move(manager)) {
    }

    explicit AbstractInputManager(GameInputManager &&manager)
        : variant_(std::move(manager)) {
    }

/// TODO: @sckorn @radameon introduce a class that incapsulates different input events (X and MS)
#ifdef __linux__
    [[nodiscard]] decltype(auto) tryProcessEvent(XEvent &event) {
        return std::visit([&event](auto &arg) mutable -> bool {
            auto ret = arg.tryProcessEvent(event);
            return ret;
        },
                          variant_);
    }
#endif
#ifdef _WIN32
    [[nodiscard]] decltype(auto) tryProcessEvent(rdmn::core::windows_input_event &event) {
        return std::visit([&event](auto &arg) mutable -> bool {
            auto ret = arg.tryProcessEvent(event);
            return ret;
        },
                          variant_);
    }
#endif

    [[nodiscard]] decltype(auto) getValue(core::MouseEventType event) const {
        return std::visit([event](const auto &m) { return m.getValue(event); }, variant_);
    }

    [[nodiscard]] decltype(auto) getValue(core::KeyboardEventType event) const {
        return std::visit([event](const auto &m) { return m.getValue(event); }, variant_);
    }

#ifdef RCBE_DEBUG_MODE
    //*For the simplicity of debug*/
    InputManagerVariant &getVariant() {
        return variant_;
    }
#endif

private:

    InputManagerVariant variant_;
};

using AbstractInputManagerPtr = std::shared_ptr<AbstractInputManager>;
using AbstractInputManagerConstPtr = std::shared_ptr<const AbstractInputManager>;
}// namespace rcbe::core

#endif//RCBE_ENGINE_ABSTRACTINPUTMANAGER_HPP
