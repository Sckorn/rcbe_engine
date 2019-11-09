#include <data_types/math/Matrix.hpp>

namespace nlohmann
{
void adl_serializer<rcbe::math::Matrix<rcbe::core::EngineScalar, 3, 3>> ::to_json(
    nlohmann::json& j, 
    const rcbe::math::Matrix<rcbe::core::EngineScalar, 3, 3>& value
)
{
    j = {
        {"rows", value.rows},
        {"columns", value.columns}
    };
}

void adl_serializer<rcbe::math::Matrix<rcbe::core::EngineScalar, 3, 3>> ::from_json(
    const nlohmann::json& j, 
    rcbe::math::Matrix<rcbe::core::EngineScalar, 3, 3>& value
)
{
    //TODO: implement when matrix is finished
}
}