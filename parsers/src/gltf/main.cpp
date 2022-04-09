#include <chrono>

#include <boost/log/trivial.hpp>

#include <rcbe-engine/parsers/gltf/gltf_parser.hpp>

#include <rcbe-engine/utils/output_utils.hpp>

auto main(int argc, char * argv[]) -> int {
    rcbe::utils::setup_logging(boost::log::trivial::trace);
    const auto gltf_path = argv[1];
    const auto bin_path = argv[2];

    try {
        auto start = std::chrono::steady_clock::now();
        const auto parsed_objects = rdmn::parse::gltf::parse(gltf_path, bin_path);
        auto end = std::chrono::steady_clock::now();

        BOOST_LOG_TRIVIAL(debug) << "Total sec spent on parsing GLTF: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        BOOST_LOG_TRIVIAL(debug) << "Total objects parsed: " << parsed_objects.size();
    } catch (const std::exception &e) {
        BOOST_LOG_TRIVIAL(fatal) << "Exception while parsing GLTF " << e.what();
        return 1;
    }

    return 0;
}