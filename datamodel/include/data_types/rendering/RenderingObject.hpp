#ifndef RCBE_RENDERINGOBJECT_HPP
#define RCBE_RENDERINGOBJECT_HPP

#include <data_types/math/math.hpp>

namespace rcbe::rendering
{

class [[deprecated]] RenderingObject
{
public:
    virtual void render() = 0;

    void SetTranslation(math::Vector3d &&translation);
    void SetOrientation(math::Matrix3x3d &&orientation);
    void setId(std::string &id);

    const math::Vector3d &GetTranslation() const;
    const math::Matrix3x3d &GetOrientation() const;
    const std::string &GetId() const;

private:
    math::Vector3d _translation;
    math::Matrix3x3d _orientation;

    std::string _id;
    std::vector<std::string> _tags;
    bool _enabled = true;
};

class RenderingPrimitive : public RenderingObject
{
public:
    
};
}

#endif