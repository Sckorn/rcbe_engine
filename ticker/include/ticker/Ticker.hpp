#ifndef RCBE_ENGINE_TICKER_HPP
#define RCBE_ENGINE_TICKER_HPP

#include <chrono>
#include <thread>
#include <mutex>
#include <functional>

#include <rcbe/traits/time_traits.hpp>

namespace rcbe::core {
class Ticker {
public:
    using period_type = std::chrono::nanoseconds ;
    using handler_type = std::function<void(void)>;
    using clock_type = std::chrono::steady_clock;

    Ticker() = delete;
    template <typename Duration, typename = std::enable_if_t <rcbe::utility::duration_type_v<Duration>, void>>
    explicit Ticker(Duration&& period, handler_type&& handler)
    :
    period_ { std::chrono::duration_cast<period_type>(period) }
    , start_time_ { Ticker::clock_type ::now() }
    , handler_ { std::move(handler) }
    {
        std::lock_guard lg { control_mutex_ };
        running_ = true;

        ticker_thread_ = std::thread([this]() {
            while (running_) {
                if (handler_)
                    handler_();
                std::this_thread::sleep_for(period_);
            }
        });
    }
    ~Ticker();

    void set_handler(handler_type&& handler);
    void stop();

    [[nodiscard]] period_type delta_time() const;
    [[nodiscard]] clock_type::time_point start_time() const;

private:

    std::mutex control_mutex_;
    bool running_ = false;
    std::thread ticker_thread_;

    period_type period_;
    handler_type handler_;
    clock_type::time_point start_time_;
};
}

#endif //RCBE_ENGINE_TICKER_HPP
