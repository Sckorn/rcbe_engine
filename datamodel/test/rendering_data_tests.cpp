#include <fstream>

#include <nlohmann/json.hpp>

#include <gtest/gtest.h>

#include <rcbe-engine/datamodel/rendering/renderer_config.hpp>
#include <rcbe-engine/utils/json_utils.hpp>

static constexpr const char *path = "datamodel/data/rendering/default_renderer_config.json";

TEST(RendererConfigTests, FromJson) {
    auto config = rcbe::utils::read_from_file<rcbe::rendering::renderer_config>(path);

    ASSERT_TRUE(config.resizable);
    ASSERT_EQ(config.initial_dimensions.width, 1280);
}