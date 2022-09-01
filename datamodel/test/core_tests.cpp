#include <gtest/gtest.h>

#include <rcbe-engine/datamodel/core/CoreObject.hpp>
#include <rcbe-engine/datamodel/core/Dimensions.hpp>

class DimensionsTests : public ::testing::Test {
public:

    [[nodiscard]] rcbe::core::Dimensions getTestDimensions() const {
        return dim;
    }

    [[nodiscard]] constexpr rcbe::core::EngineIntergral initialWidth() const {
        return IWIDTH;
    }

    [[nodiscard]] constexpr rcbe::core::EngineIntergral initialHeight() const {
        return IHEIGHT;
    }

private:
    static constexpr rcbe::core::EngineIntergral IWIDTH = 1024;
    static constexpr rcbe::core::EngineIntergral IHEIGHT = 768;
    static constexpr rcbe::core::Dimensions dim{IWIDTH, IHEIGHT};
};

using CoreObjectTests = DimensionsTests;

namespace {
struct some_empty_struct {};
}

TEST_F(DimensionsTests, IntegerDimensions) {
    const auto dim = getTestDimensions();

    ASSERT_EQ(dim.width, initialWidth());
    ASSERT_EQ(dim.height, initialHeight());
}

TEST_F(DimensionsTests, DoubleDimensions) {
    using ScalarT = rcbe::core::ScalarDimensions::ValueType;
    const auto sdim = getTestDimensions().convertUnerlying<ScalarT>();

    ASSERT_EQ(sdim.width, static_cast<ScalarT>(initialWidth()));
    ASSERT_EQ(sdim.height, static_cast<ScalarT>(initialHeight()));
}

TEST_F(CoreObjectTests, Components) {
    using DimT = rcbe::core::Dimensions;

    rcbe::core::CoreObject co{some_empty_struct{}};

    auto dim = getTestDimensions();
    co.addComponent(std::move(dim));

    ASSERT_TRUE(co.hasComponent<DimT>());
    const auto dim_comp_ptr = co.getComponent<DimT>();
    ASSERT_NE(dim_comp_ptr, nullptr);
    
    const auto &dim_comp = dim_comp_ptr->as<DimT>();
    ASSERT_EQ(dim_comp.width, initialWidth());
    ASSERT_EQ(dim_comp.height, initialHeight());
}

TEST_F(CoreObjectTests, Tags) {
    static constexpr const char * TAG = "systag";
    rcbe::core::CoreObject co{some_empty_struct{}};

    ASSERT_TRUE(co.addSystemTag(std::string(TAG)));
    ASSERT_TRUE(co.hasSystemTag(std::string(TAG)));
}