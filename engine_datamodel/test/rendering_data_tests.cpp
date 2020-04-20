#include <gtest/gtest.h>

#include <data_types/rendering/RendererConfig.hpp>
#include <nlohmann/json.hpp>

#include <fstream>

#include <common/utils/json_utils.hpp>

static constexpr const char * path = "engine_datamodel/data/rendering/default_renderer_config.json";

TEST(RendererConfigTests, FromJson) {
    auto config = rcbe::utils::readFromFile<rcbe::rendering::RendererConfig>(path);

    ASSERT_TRUE(config.resizable);
    ASSERT_EQ(config.initial_dimensions.width, 1280);
}