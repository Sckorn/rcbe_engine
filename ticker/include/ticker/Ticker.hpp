#ifndef RCBE_ENGINE_TICKER_HPP
#define RCBE_ENGINE_TICKER_HPP

#include <chrono>
#include <thread>
#include <mutex>
#include <functional>

#include <rcbe/traits/time_traits.hpp>

#include <boost/log/trivial.hpp>

namespace rcbe::core {
class Ticker {
public:
    using period_type = std::chrono::nanoseconds;
    using handler_type = std::function<void(void)>;
    using clock_type = std::chrono::steady_clock;

    Ticker() = delete;
    template <typename Duration, typename = std::enable_if_t <rcbe::utility::duration_type_v<Duration>, void>>
    explicit Ticker(Duration&& period, handler_type&& handler)
    :
    period_ { std::chrono::duration_cast<period_type>(period) }
    , handler_ { std::move(handler) }
    , start_time_ { Ticker::clock_type ::now() }
    , running_ (true)
    , ticker_thread_([this]() {
        while (running_) {
            BOOST_LOG_TRIVIAL(debug) << std::boolalpha << running_ << " thread id " << std::this_thread::get_id();
            if (handler_ && running_)
                handler_();
            std::this_thread::sleep_for(period_);
        }
    })
    {}
    ~Ticker();

    Ticker(const Ticker& other) = delete;
    Ticker &operator=(const Ticker& other) = delete;

    Ticker(Ticker&& other) = default;
    Ticker &operator=(Ticker&& other) = default;

    void set_handler(handler_type&& handler);
    void stop();

    [[nodiscard]] period_type delta_time() const;
    [[nodiscard]] clock_type::time_point start_time() const;

    void wait();

private:

    std::mutex control_mutex_;
    std::mutex join_mutex_;
    bool running_ = false;
    std::thread ticker_thread_;

    period_type period_;
    handler_type handler_;
    clock_type::time_point start_time_;
};
}

#endif //RCBE_ENGINE_TICKER_HPP
