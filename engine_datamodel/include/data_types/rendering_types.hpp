#ifndef RCBE_DATA_TYPES_RENDERING_TYPES
#define RCBE_DATA_TYPES_RENDERING_TYPES

#include <data_types/core_types.hpp>

namespace rcbe::data_types::rendering
{
//using 

class RenderingObject
{
public:
    virtual void render() = 0;

    void SetTranslation(core::Vector3d &&translation);
    void SetOrientation(core::Matrix3x3d &&orientation);
    void setId(std::string &id);

    const core::Vector3d &GetTranslation() const;
    const core::Matrix3x3d &GetOrientation() const;
    const std::string &GetId() const;

private:
    core::Vector3d _translation;
    core::Matrix3x3d _orientation;

    std::string _id;
    std::vector<std::string> _tags;
    bool _enabled = true;
};

class RenderingPrimitive : public RenderingObject
{
public:
    
};
}

#endif //RCBE_DATA_TYPES_RENDERING_TYPES