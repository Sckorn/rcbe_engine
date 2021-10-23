#!/usr/bin/python

import sys
import os
import shutil
import argparse
from shutil import copyfile

def parse_arguments(argv):
    parser = argparse.ArgumentParser(description="RCBE deb packages deploy script")
    parser.add_argument('-d', '--packages-directory', default='bazel-bin', help='Path to directory with built deb pacakges')
    parser.add_argument('-e', '--engine-version', default='0.0.4', help='Engine version for which deb packages were built')
    parser.add_argument('-a', '--target-architecture', default='amd64', help='Engine target architecture for which deb pacakges were built')
    parser.add_argument('-o', '--output-directory', default='/tmp', help='Output directory where a tar.gz file containing all deployed deb pacakges will be created')
    parser.add_argument('-p', '--packages-extension', default='deb', help='Extension of packages to be deployed')

    parsed_args = parser.parse_args(argv)

    return parsed_args.packages_directory, parsed_args.engine_version, parsed_args.target_architecture, parsed_args.output_directory, parsed_args.packages_extension

def clear_dir(path):
    for filename in os.listdir(path):
        file_path = os.path.join(path, filename)
        try:
            if os.path.isfile(file_path) or os.path.islink(file_path):
                os.unlink(file_path)
            elif os.path.isdir(file_path):
                shutil.rmtree(file_path)
        except Exception as e:
            print('Failed to delete %s. Reason: %s' % (file_path, e))

def clear_and_rm_dir(tmp_dir):
    try:
        clear_dir(tmp_dir)
        os.rmdir(tmp_dir)
    except OSError:
        print ("Deletion of the directory %s failed" % tmp_dir)
        if len(os.listdir(tmp_dir) ) == 0:
            print("Directory is empty")
        else:
            print("Directory is not empty")
    else:
        print ("Successfully deleted the directory %s " % tmp_dir)

def scan_dir_for_packages(folder, engine_version, target_arch, extensions = ("deb")):
    return [os.path.join(r, fn)
            for r, ds, fs in os.walk(folder)
            for fn in fs if fn.endswith(extensions) and fn.rfind(engine_version) != -1 and fn.rfind(target_arch) != -1]

def main(packages_path, engine_version, target_arch, output_file_path, extension):
    cwd = os.getcwd()
    print("CWD: {}".format(cwd))
    deb_files_path = os.path.join(cwd, str(packages_path))
    print("Trying to deploy version {}".format(engine_version))
    print("For arch {}".format(target_arch))
    print("Starting bazel build for deploy")
    bazel_exit_code = os.system("bazel build //...")  # the simplest way is to build packages prior to packaging
                                    # , and since there is no tag value for packages, we are building a;l

    if (bazel_exit_code != 0):
        print("Error while building packages")
        exit(1)

    print("Searching in {}".format(deb_files_path))

    found_deb_files = scan_dir_for_packages(deb_files_path, engine_version, target_arch, extension)

    print("Total found deb files {}".format(found_deb_files))


    tmp_dir = '/tmp/deploy_{}_{}'.format(engine_version, target_arch)
    print("Creating temp dir for deploy: {}".format(tmp_dir))

    try:
        os.mkdir(tmp_dir)
    except OSError:
        print ("Creation of the directory %s failed" % tmp_dir)
        exit(1)
    else:
        print ("Successfully created the directory %s " % tmp_dir)

    for fp in found_deb_files:
        print("Copying file: {}".format(fp))
        basename = os.path.basename(fp)
        full_new_name = os.path.join(tmp_dir, basename)
        print("To {}".format(full_new_name))
        copyfile(fp, full_new_name)

    print("Packing the temp directory into tar.gz")

    output_file_path = os.path.join(output_file_path, "rcbe_engine_{}_{}.tar.gz".format(engine_version, target_arch))

    tar_ec = os.system('tar -czvf {} -C {} .'.format(output_file_path, tmp_dir))
    if tar_ec != 0:
        print("Can't create tar archive")
        try:
            clear_and_rm_dir(tmp_dir)
        except OSError:
            print("Can't cleanup")
        exit(tar_ec)

    print("Deleting temp dir for deploy: {}".format(tmp_dir))
    clear_and_rm_dir(tmp_dir)

    print("Deploy finished!")

if __name__ == "__main__":
    source_dir, engine_version, target_arch, output_file, extension = parse_arguments(sys.argv[1:])

    main(source_dir, engine_version, target_arch, output_file, extension)
