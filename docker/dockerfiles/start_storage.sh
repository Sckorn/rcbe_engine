#!/bin/sh

cd /rcbe_engine/rcbe_static_storage/
bazel run //web_server:webserver -- --port 8080 --share /exchange/storage/ &