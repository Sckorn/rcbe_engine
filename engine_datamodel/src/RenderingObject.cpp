#include <data_types/rendering/RenderingObject.hpp>

namespace rcbe::rendering
{
void RenderingObject::SetTranslation(geometry::Vector3d &&translation)
{
    _translation = translation;
}

void RenderingObject::SetOrientation(geometry::Matrix3x3d &&orientation)
{
    _orientation = orientation;
}

void RenderingObject::setId(std::string &id)
{
    _id = id;
}

const geometry::Vector3d &RenderingObject::GetTranslation() const
{
    return _translation;
}

const geometry::Matrix3x3d &RenderingObject::GetOrientation() const
{
    return _orientation;
}

const std::string &RenderingObject::GetId() const
{
    return _id;
}
}