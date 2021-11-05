#!/bin/sh

cd /radameon/web_server/rcbe_static_storage-master/
bazel run //web_server:webserver -- --port 8080 --share /exchange/storage/ &