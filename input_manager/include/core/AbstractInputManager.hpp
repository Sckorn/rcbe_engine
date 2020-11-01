#ifndef RCBE_ENGINE_ABSTRACTINPUTMANAGER_HPP
#define RCBE_ENGINE_ABSTRACTINPUTMANAGER_HPP

#include <memory>
#include <variant>

#include <nlohmann/json_fwd.hpp>

#include <boost/log/trivial.hpp>

#include <core/InputManagerImplementation.hpp>
#include <data_types/rendering/RenderingContext.hpp>

#include <traits/input_manager.hpp>

namespace rcbe::core {

// TODO: refactor for ptrs
using AbstractInputManagerPtr = std::shared_ptr<InputManagerImplementation>;
using AbstractInputManagerConstPtr = std::shared_ptr<const InputManagerImplementation>;

AbstractInputManagerPtr create_input_manager(const rendering::RenderingContextPtr& ctx);
AbstractInputManagerPtr create_input_manager(nlohmann::json&& j);
}

#endif //RCBE_ENGINE_ABSTRACTINPUTMANAGER_HPP
