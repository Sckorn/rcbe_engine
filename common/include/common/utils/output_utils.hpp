#ifndef RCBE_ENGINE_OUTPUT_UTILS_HPP
#define RCBE_ENGINE_OUTPUT_UTILS_HPP

#include <rdmn-engine/logger/LoggerType.hpp>
#include <rdmn-engine/public_api.hpp>

namespace rcbe::utils {
R_PUBLIC_API void setup_logging(rdmn::core::log::LoggerType severity = rdmn::core::log::LoggerType::info);
}

#endif//RCBE_ENGINE_OUTPUT_UTILS_HPP
