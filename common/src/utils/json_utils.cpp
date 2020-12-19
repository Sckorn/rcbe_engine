#include <rcbe-engine/utils/json_utils.hpp>

namespace rcbe::utils {
nlohmann::json read_raw(const std::string &path) {
    std::ifstream ifs(path);

    if(!ifs)
        throw std::runtime_error("File doesn't exist or malfromed!");

    nlohmann::json j;
    ifs >> j;
    return j;
}
}