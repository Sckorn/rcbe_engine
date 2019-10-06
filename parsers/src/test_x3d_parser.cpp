#include <parsers/x3d/X3dParser.hpp>

int  main(int argc, char * argv[])
{
    const auto meshes = rcbe::parsers::parseMeshes("/home/oper/dev/rcbe_static_storage/corner.x3d");

    if (meshes.size() != 1)
        return 1;

    return 0;
}