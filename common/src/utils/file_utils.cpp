#include <rcbe-engine/utils/file_utils.hpp>

namespace rcbe::utils {
std::filesystem::path make_path_from_parts(const std::vector<std::string> &parts) {
    std::filesystem::path ret;
    for (const auto &part : parts) {
        ret /= std::filesystem::path(part);
    }
    return ret;
}

std::string make_string_path_from_parts(const std::vector<std::string> &parts) {
    return make_path_from_parts(parts).string();
}
}// namespace rcbe::utils