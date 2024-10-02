#ifndef RDMN_GLOG_OUTPUT_UTILS_HPP
#define RDMN_GLOG_OUTPUT_UTILS_HPP

#include <iomanip>

#include <glog/logging.h>

namespace {
void formatter(std::ostream& s, const google::LogMessage& m, void* /*data*/) {
    using namespace std;
    auto sev = google::GetLogSeverityName(m.severity())[0];
    switch (sev) {
        case 'I':
        s << "\033[36m" << "[INFO]" << ' ';
        break;
        case 'W':
        s << "\033[1;35m" << "[WARN]" << ' ';
        break;
        case 'E':
        s << "\033[31m" << "[ERROR]" << ' ';
        break;
        case 'F':
        s << "\033[31m" << "[FATAL]" << ' ';
        break;
        default:
        break;
    }

    s << setw(4) << 1900 + m.time().year()
    << setw(2) << 1 + m.time().month()
    << setw(2) << m.time().day()
    << ' '
    << setw(2) << m.time().hour() << ':'
    << setw(2) << m.time().min()  << ':'
    << setw(2) << m.time().sec() << "."
    << setw(6) << m.time().usec()
    << ' '
    << setfill(' ') << setw(5)
    << m.thread_id() << setfill('0')
    << ' '
    << m.basename() << ':' << m.line() << "]" << "\033[0m";
}
}

namespace rcbe::utils {

R_PUBLIC_API void setup_logging(rdmn::core::log::LoggerType severity) {
    google::InstallPrefixFormatter(&formatter);
}

}



#endif