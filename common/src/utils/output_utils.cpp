#include <rcbe-engine/utils/output_utils.hpp>

/**
 * Terminal colors explanation
 *
 * _____________________________________________________________
 *  | Parameter  | Effect                                      |
 *  ____________________________________________________________
 *  | 0          | reset all SGR effects to their default      |
 *  | 1          | bold or increased intensity                 |
 *  | 2          | faint or decreased intensity                |
 *  | 4          | singly underlined                           |
 *  | 5          | slow blink                                  |
 *  | 30-37      | foreground color (8 colors)                 |
 *  | 38;5;x     | foreground color (256 colors, non-standard) |
 *  | 38;2;r;g;b | foreground color (RGB, non-standard)        |
 *  | 40-47      | background color (8 colors)                 |
 *  | 48;5;x     | background color (256 colors, non-standard) |
 *  | 48;2;r;g;b | background color (RGB, non-standard)        |
 *  | 90-97      | bright foreground color (non-standard)      |
 *  | 100-107    | bright background color (non-standard)      |
 *  ____________________________________________________________
 *
 *  The 8 actual colors within the ranges (30-37, 40-47, 90-97, 100-107) are defined by the ANSI standard as follows:
 *  -----------------------------
 *  | Last Digit    |   Color   |
 *  -----------------------------
 *  | 0             |   black   |
 *  | 1             |   red     |
 *  | 2             |   green   |
 *  | 3             |   yellow  |
 *  | 4             |   blue    |
 *  | 5             |   magenta |
 *  | 6             |   cyan    |
 *  | 7             |   white   |
 *  -----------------------------
 *
 * */

namespace {
void coloring_formatter(boost::log::record_view const &rec, boost::log::formatting_ostream &strm) {
    namespace logging_expr = boost::log::expressions;

    auto severity = rec[boost::log::trivial::severity];
    if (severity) {
        // Set the color
        switch (severity.get()) {
            case boost::log::trivial::info:
                strm << "\033[32m";
                break;
            case boost::log::trivial::warning:
                strm << "\033[93m";
                break;
            case boost::log::trivial::error:
                strm << "\033[31m";
                break;
            case boost::log::trivial::fatal:
                strm << "\033[41m";
                break;
            case boost::log::trivial::debug:
                strm << "\033[36m";
                break;
            case boost::log::trivial::trace:
                strm << "\033[34m";
                break;
            default:
                break;
        }
    }

    strm << " [" << rec[boost::log::trivial::severity] << "] " << rec[boost::log::expressions::smessage];
    if (severity) {
        // Restore the default color
        strm << "\033[0m";
    }
}
}// namespace

namespace rcbe::utils {

void setup_logging(boost::log::trivial::severity_level minimal_visible_level) {
    BOOST_LOG_FUNCTION();

    boost::log::core::get()->set_filter(
        boost::log::trivial::severity >= minimal_visible_level);

    namespace logging_expr = boost::log::expressions;
    namespace logging_sinks = boost::log::sinks;
    boost::log::core::get()->add_global_attribute("TimeStamp", boost::log::attributes::local_clock());
    boost::log::core::get()->add_global_attribute("LineID", boost::log::attributes::counter<unsigned int>(1));
    boost::log::core::get()->add_global_attribute("ThreadID", boost::log::attributes::current_thread_id());
    boost::log::core::get()->add_global_attribute("ProcessID", boost::log::attributes::current_process_id());
    boost::log::core::get()->add_global_attribute("Scope", boost::log::attributes::named_scope());
    boost::log::core::get()->add_thread_attribute("Scope", boost::log::attributes::named_scope());

    boost::shared_ptr<logging_sinks::synchronous_sink<logging_sinks::text_ostream_backend>> stdout_sink {
        boost::make_shared<logging_sinks::synchronous_sink<logging_sinks::text_ostream_backend>>()};
    stdout_sink->locked_backend()->auto_flush(true);
    stdout_sink->locked_backend()->add_stream(boost::shared_ptr<std::ostream>(&std::cout, boost::null_deleter {}));

    boost::log::formatter stdout_formatter {
        logging_expr::stream
        << logging_expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "[%Y-%m-%d %H:%M:%S.%f] ")
        << logging_expr::wrap_formatter(&coloring_formatter)};

    stdout_sink->set_formatter(stdout_formatter);
    stdout_sink->set_filter(boost::log::trivial::severity >= minimal_visible_level);

    // "register" sink
    boost::log::core::get()->add_sink(stdout_sink);
}
}// namespace rcbe::utils