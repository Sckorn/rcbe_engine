#!/usr/bin/python

#
# Can't use this one from bazel due to inability to provide files to build context,
# as actual files instead of symlinks (see bazels's filegroups default).
# Also according to these:
# https://stackoverflow.com/questions/31881904/docker-follow-symlink-outside-context
# https://github.com/docker/docker/issues/1676
# This script is unusable through the means of bazel
# 

import sys
import os
import subprocess
import argparse
import json


def parse_arguments(argv):
    parser = argparse.ArgumentParser(description="Radameon docker build script")
    parser.add_argument('-d', '--docker-context', help='Path to directory containing docker files (i.e. build context)')
    parser.add_argument('-f', '--docker-file', default='', help='Path to specific docker file to build, if left empty, builds all of them')
    parser.add_argument('-r', '--relation-file', default='', help='Path to relations file')
    parser.add_argument('-v', '--verbose', action="store_true", help='Switch on verbose mode')

    parsed_args = parser.parse_args(argv)

    return parsed_args.docker_context, parsed_args.docker_file, parsed_args.relation_file, parsed_args.verbose


def tag_image(image_digest, image_name, tag, verbose=False):
    print('tagging image')
    tag_cmd = ["docker", "tag", image_digest, "radameon/{}:{}".format(image_name, tag)]
    ret = subprocess.run(tag_cmd)
    if (ret.returncode == 0):
        print("Image tagged successfully")
        return True
    else:
        print("Couldn't tag docker image")
        if verbose:
            print(ret.args)
            print(ret.stdout)
            print(ret.stderr)
        return False


def build_file(context, file, image_config, verbose=False):
    print("Building single docker file")

    if not os.path.isfile(file):
        print("Supplied file path is not an actual file")
        return False

    if not os.path.isdir(context):
        print("Supplied context path is not a dockerfiles directory")
        return False

    build_cmd = ["docker", "build"]
    if "args" in image_config.keys():

        for k in image_config["args"]:
            build_cmd += ["--build-arg"]
            build_cmd += [k + "=" + image_config["args"][k]]

    build_cmd += ["-f", file, context]

    ret = subprocess.run(build_cmd, capture_output=True, text=True)

    if ret.returncode != 0:
        print("Docker build returned non-zero code")
        if verbose:
            print(ret.stderr)
            print(ret.stdout)
        return False

    print("Docker build completed with zero retcode, processing the output")

    success_marker = "Successfully built "
    suc_string = ret.stdout.split('\n')[-2]
    substr_ind = suc_string.find(success_marker)

    resulting_digest = suc_string[substr_ind + len(success_marker):-1]
    print("Resulting digest %s" % resulting_digest)

    if image_config:
        print("Image config supplied, using it to configure the built image")
        tag_image(resulting_digest, image_config["name"], image_config["tag"])
    else:
        print("No image config provided")
        image_name = os.path.basename(file).split('.')[0]
        tag_image(resulting_digest, image_name, "latest")

    print("Building file done successfully")

    return True


def filter_dockerfile(files, relations_config, verbose=False):
    to_remove = list()
    for f in files:
        bname = os.path.basename(f)
        if bname in relations_config.keys():
            if "enabled_by" in relations_config[bname]:
                cond = relations_config[bname]["enabled_by"]["condition"]
                val = relations_config[bname]["enabled_by"]["value"]

                if cond in relations_config.keys():
                    if relations_config[cond] != val:
                        to_remove.append(f)
                else:
                    to_remove.append(f)

        else:
            to_remove.append(f)

    for r in to_remove:
        files.remove(r)
    return files


def build_context(context, relations_config, verbose=False):
    print("Building docker files images from context")
    docker_files = []
    for (dirpath, dirnames, filenames) in os.walk(context):
        for f in filenames:
            abs_file_path = os.path.join(context, f)
            print("Processing %s" % f)
            if os.path.isfile(abs_file_path) and os.path.basename(f).split(".")[1] == "docker" and os.path.basename(f) in relations_config.keys():
                docker_files.append(abs_file_path)


    initial_docker = {}
    for df in docker_files:
        df_basename = os.path.basename(df)
        if not relations_config[df_basename]["depends"]:
            print("Found docker config with empty depdends, this is the base")
            initial_docker = df

    print("Initial docker %s" % initial_docker)
    if build_file(context, initial_docker, relations_config[os.path.basename(initial_docker)], verbose):
        print("%s build successfully" % initial_docker)
        docker_files.remove(initial_docker)

    docker_files = filter_dockerfile(docker_files, relations_config)

    while docker_files:
        for df in docker_files:
            print("Trying to process %s" % df)
            df_basename = os.path.basename(df)
            current_dep = relations_config[df_basename]["depends"]
            if "condition" in relations_config[df_basename].keys():
                current_dep = current_dep.format(relations_config[relations_config[df_basename]["condition"]])

            if current_dep == os.path.basename(initial_docker):
                print("Found dependant %s from %s building it" % (df, initial_docker))
                if build_file(context, df, relations_config[df_basename], verbose):
                    print("Built %s successfully" % df)
                    initial_docker = df
                    docker_files.remove(initial_docker)
                else:
                    print("Can't build %s" % df)
                    sys.exit(1)

    print("Contextual build completed successfully!")


def main(context, file, relations, verbose):
    abs_context = os.path.abspath(context)
    abs_file = os.path.abspath(file)
    abs_relations = os.path.abspath(relations)

    if verbose:
        print("Context %s" % abs_context)
        print("File %s" % abs_file)
        print("Relations file %s" % abs_relations)

    relations_config = None
    if os.path.exists(abs_relations) and os.path.isfile(abs_relations):
        print("Parsing relations config")
        relations_config = json.load(open(abs_relations, "r"))

    if file:
        print("Docker file supplied to build %s" % file)
        if relations_config:
            build_file(abs_context, abs_file, relations_config[os.path.basename(abs_file)], verbose)
        else:
            build_file(abs_context, abs_file, None, verbose)
    else:
        print("Building from docker context %s" % context)
        build_context(abs_context, relations_config, verbose)


if __name__ == "__main__":
    context, file, relations, verbose = parse_arguments(sys.argv[1:])

    if verbose:
        print('Verbose mode activated')

    main(context, file, relations, verbose)
