#include <data_types/rendering_types.hpp>

namespace rcbe::data_types::rendering
{
void RenderingObject::SetTranslation(core::Vector3d &&translation)
{
    _translation = translation;
}

void RenderingObject::SetOrientation(core::Matrix3x3d &&orientation)
{
    _orientation = orientation;
}

void RenderingObject::setId(std::string &id)
{
    _id = id;
}

const core::Vector3d &RenderingObject::GetTranslation() const
{
    return _translation;
}

const core::Matrix3x3d &RenderingObject::GetOrientation() const
{
    return _orientation;
}

const std::string &RenderingObject::GetId() const
{
    return _id;
}
}