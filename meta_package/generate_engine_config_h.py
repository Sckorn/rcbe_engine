#!/usr/bin/python
# -*- coding: UTF-8 -*-

import sys
import argparse
import io
import hashlib
import datetime

def parse_arguments(argv):
    parser = argparse.ArgumentParser(description="Engine config header files generator arguments")
    parser.add_argument('-M', '--major-version', help='Current engine major version')
    parser.add_argument('-m', '--minor_version', help='Current engine minor version')
    parser.add_argument('-p', '--patch-version', help='Current engine patch version')
    parser.add_argument('-u', '--project-url', help='Project website URL')
    parser.add_argument('-o', '--project-maintainer', help='Project maintainer email and name')

    parsed_args = parser.parse_args(argv)

    return parsed_args.major_version, parsed_args.minor_version, parsed_args.patch_version, parsed_args.project_url, parsed_args.project_maintainer

def main(major, minor, patch, url, maintainer):
    build_hash = int(abs(hash(datetime.datetime.now())) % (10 ** 8))
    print("Engine full version {}.{}.{}.{}".format(major, minor, patch, build_hash))
    try:
        my_file_handle = open("rcbe_engine_config.h", mode="w")
        my_file_handle.write("#ifndef RCBE_ENGINE_CONFIG_H\n")
        my_file_handle.write("#define RCBE_ENGINE_CONFIG_H\n\n")
        my_file_handle.write("#include <string>\n\n")
        my_file_handle.write("static constexpr const char * RCBE_ENGINE_MAJOR_VERSION = \"{}\";\n".format(major))
        my_file_handle.write("static constexpr const char * RCBE_ENGINE_MINOR_VERSION = \"{}\";\n".format(minor))
        my_file_handle.write("static constexpr const char * RCBE_ENGINE_PATCH_VERSION = \"{}\";\n".format(patch))
        my_file_handle.write("static constexpr const char * RCBE_ENGINE_BUILD_VERSION = \"{}\";\n".format(build_hash))
        my_file_handle.write("static constexpr const char * RCBE_ENGINE_PROJECT_URL = \"{}\";\n".format(url))
        my_file_handle.write("static constexpr const char * RCBE_ENGINE_PROJECT_MAINTAINER = \"{}\";\n\n".format(maintainer))
        my_file_handle.write("static std::string getEngineVersionWoBuild() {\n")

        my_file_handle.write("    return std::string(RCBE_ENGINE_MAJOR_VERSION) "
                             "+ \".\" + std::string(RCBE_ENGINE_MINOR_VERSION) + \".\" "
                             "+ std::string(RCBE_ENGINE_PATCH_VERSION);"
                             "\n")
        my_file_handle.write("}\n\n")

        my_file_handle.write("static std::string getEngineVersionFull() {\n")
        my_file_handle.write("    return getEngineVersionWoBuild() + \".\" + std::string(RCBE_ENGINE_BUILD_VERSION);\n")
        my_file_handle.write("}\n\n")

        my_file_handle.write("static std::string getMaintainerString() {\n")
        my_file_handle.write("    return std::string(RCBE_ENGINE_PROJECT_MAINTAINER);\n")
        my_file_handle.write("}\n\n")

        my_file_handle.write("static std::string getProjectUrlString() {\n")
        my_file_handle.write("    return std::string(RCBE_ENGINE_PROJECT_URL);\n")
        my_file_handle.write("}\n\n")

        my_file_handle.write("#endif // RCBE_ENGINE_CONFIG_H\n")
        my_file_handle.close()
        
    except IOError:
        print("File not found or path is incorrect")
        
    print("Done generating engine config")

if __name__ == "__main__":
    engine_version_major, engine_version_minor, engine_version_patch, project_url, maintainer = parse_arguments(sys.argv[1:])

    main(engine_version_major, engine_version_minor, engine_version_patch, project_url, maintainer)
