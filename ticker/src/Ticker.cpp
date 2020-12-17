#include <ticker/Ticker.hpp>

#include <iostream>

namespace rcbe::core {

Ticker::~Ticker() {
    if (running_) {
        stop();
    }
    wait();
}

Ticker::PeriodType Ticker::deltaTime() const {
    return period_;
}

Ticker::ClockType ::time_point Ticker::startTime() const {
    return start_time_;
}

void Ticker::stop() {
    std::lock_guard lg { control_mutex_ };
    running_ = false;
    BOOST_LOG_TRIVIAL(debug) << "Ticker is stopped " << std::boolalpha << running_;
}

void Ticker::setHandler(rcbe::core::Ticker::HandlerType &&handler) {
    handler_ = std::move(handler);
}

void Ticker::wait() {
    BOOST_LOG_TRIVIAL(debug) << "Waiting for ticker to stop...";
    std::lock_guard lg { join_mutex_ };
    if (ticker_thread_.joinable()) {
        ticker_thread_.join();
    }
    else { // this way is wrong on so many levels, will remove it later
        while(running_);
    }
    BOOST_LOG_TRIVIAL(debug) << "Ticker stopped.";
}
}
