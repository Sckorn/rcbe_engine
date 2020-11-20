#!/usr/bin/python

import sys
import os
import shutil
from shutil import copyfile

## TODO: add proper command line args prase and usage
def usage():
    print("deploy_packages.py")
    print("A simple python script for engine deb packages build and deployment.")
    print('Usage:')
    print('python3 ./deploy_packages.py root/directory/with/built/packages [bazel-bin] required-engine-version [0.0.3] target-architecture [amd64] resulting-tar-path [/tmp/deploy.tar.gz]')

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

def scan_dir_for_deb(folder, engine_version, target_arch, extensions = ("deb")):
    return [os.path.join(r, fn)
            for r, ds, fs in os.walk(folder)
            for fn in fs if fn.endswith(extensions) and fn.rfind(engine_version) != -1 and fn.rfind(target_arch) != -1]

def main():
    cwd = os.getcwd()
    print("CWD: {}".format(cwd))
    deb_files_path = os.path.join(cwd, str(sys.argv[1]))
    engine_version = sys.argv[2]
    target_arch = sys.argv[3]
    print("Trying to deploy version {}".format(engine_version))
    print("For arch {}".format(target_arch))
    print("Starting bazel build for deploy")
    bazel_exit_code = os.system("bazel build //...")  # the simplest way is to build packages prior to packaging
                                    # , and since there is no tag value for packages, we are building a;l

    if (bazel_exit_code != 0):
        print("Error while building packages")
        exit(1);

    print("Searching in {}".format(deb_files_path))

    found_deb_files = scan_dir_for_deb(deb_files_path, engine_version, target_arch)

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
    output_file_path = sys.argv[4]

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
    if len(sys.argv) != 5:
        usage()
        exit(1)
    main()
