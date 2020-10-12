#include <chrono>

#include <boost/log/trivial.hpp>

#include <core/InputManagerImplementation.hpp>

namespace rcbe::core {

bool InputManagerImplementation::try_process_event(XEvent& event) {
    auto event_type = static_cast<InputEventType>(event.type);
    auto it = handlers_.find(event_type);
    if (it != handlers_.end()) {
        propagated_events_.push(event);
        active_events_[event.type] = true;

        auto itt = exclusive_events_.find(event_type);
        if (itt != exclusive_events_.end()) {
            active_events_[static_cast<int>(itt->second)] = false;
        }

        try {
            it->second.invoke(propagated_events_);
        } catch (const std::exception& e) {
            BOOST_LOG_TRIVIAL(error) << "Exception: " << e.what();
            return false;
        }

        return true;
    } else {
        BOOST_LOG_TRIVIAL(warning) << "Can't find handler for event type " << event.type;
        return false;
    }
}

bool InputManagerImplementation::event_active(InputEventType event_type) const {
    auto intetype = static_cast<int>(event_type);
    if (active_events_.find(intetype) != active_events_.end())
    {
        return active_events_.at(intetype);
    }
    return false;
}
}