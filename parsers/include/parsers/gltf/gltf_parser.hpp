#ifndef RCBE_ENGINE_GLTF_PARSER_HPP
#define RCBE_ENGINE_GLTF_PARSER_HPP

#include <optional>
#include <vector>

#include <rcbe-engine/fundamentals/types.hpp>
#include <rcbe-engine/datamodel/core/CoreObject.hpp>

namespace rdmn::parse::gltf {
std::vector<rcbe::core::CoreObject> parse(const rcbe::core::EnginePath &gltf_file_path, const rcbe::core::EnginePath &bin_file_path);
}

#endif //RCBE_ENGINE_GLTF_PARSER_HPP
