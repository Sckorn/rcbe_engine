#ifndef RCBE_ENGINE_TICKER_HPP
#define RCBE_ENGINE_TICKER_HPP

#include <chrono>
#include <functional>
#include <mutex>
#include <thread>

#include <rdmn-engine/logger/trivial_logger.hpp>

#include <rcbe-engine/traits/time_traits.hpp>

namespace rcbe::core {
class R_PUBLIC_API Ticker {
public:

    using PeriodType = std::chrono::nanoseconds;
    using HandlerType = std::function<void(void)>;
    using ClockType = std::chrono::steady_clock;

    Ticker() = delete;
    template <typename Duration, typename = std::enable_if_t<rcbe::utility::duration_type_v<Duration>, void>>
    explicit Ticker(Duration &&period, HandlerType &&handler)
        : period_ {std::chrono::duration_cast<PeriodType>(period)}
        , handler_ {std::move(handler)}
        , start_time_ {Ticker::ClockType ::now()}
        , running_(true)
        , ticker_thread_([this]() {
            while (running_) {
                RDMN_LOG(RDMN_LOG_DEBUG) << std::boolalpha << running_ << " thread id " << std::this_thread::get_id();
                if (handler_ && running_)
                    handler_();
                std::this_thread::sleep_for(period_);
            }
        }) {}
    ~Ticker();

    Ticker(const Ticker &other) = delete;
    Ticker &operator=(const Ticker &other) = delete;

    Ticker(Ticker &&other) = default;
    Ticker &operator=(Ticker &&other) = default;

    void setHandler(HandlerType &&handler);
    void stop();

    [[nodiscard]] PeriodType deltaTime() const;
    [[nodiscard]] ClockType::time_point startTime() const;

    void wait();

private:

    std::mutex control_mutex_;
    std::mutex join_mutex_;
    bool running_ = false;
    std::thread ticker_thread_;

    PeriodType period_;
    HandlerType handler_;
    ClockType::time_point start_time_;
};
}// namespace rcbe::core

#endif//RCBE_ENGINE_TICKER_HPP
