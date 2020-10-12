#ifndef RCBE_ENGINE_EDITORINPUTMANAGER_HPP
#define RCBE_ENGINE_EDITORINPUTMANAGER_HPP

#include <memory>
#include <enable_shared_from_this.hpp>

#include <core/InputManagerImplementation.hpp>

#include <data_types/rendering/RenderingContext.hpp>

namespace rcbe::core {
class EditorInputManager final : protected InputManagerImplementation {
public:
    EditorInputManager() = delete;
    explicit EditorInputManager(const rcbe::rendering::RenderingContextPtr& rendering_context);
    ~EditorInputManager() = default;
    EditorInputManager(const EditorInputManager& other) = delete;
    EditorInputManager(EditorInputManager&& other) = default;
    EditorInputManager& operator=(const EditorInputManager& other) = delete;
    EditorInputManager& operator=(EditorInputManager&& other) = default;

    bool try_process_event(InputManagerImplementation::input_event_reference event);

    void init();

private:

    bool methods_initialized_ = false;
    rendering::RenderingContextPtr rendering_context_ = nullptr;
};
}

#endif //RCBE_ENGINE_EDITORINPUTMANAGER_HPP
