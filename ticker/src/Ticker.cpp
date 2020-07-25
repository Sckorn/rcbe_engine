#include <ticker/Ticker.hpp>

namespace rcbe::core {

Ticker::~Ticker() {
    stop();

    if (ticker_thread_.joinable()) {
        ticker_thread_.join();
    }
}

Ticker::period_type Ticker::delta_time() const {
    return period_;
}

Ticker::clock_type ::time_point Ticker::start_time() const {
    return start_time_;
}

void Ticker::stop() {
    std::lock_guard lg { control_mutex_ };
    running_ = false;
}

void Ticker::set_handler(rcbe::core::Ticker::handler_type &&handler) {
    handler_ = std::move(handler);
}
}