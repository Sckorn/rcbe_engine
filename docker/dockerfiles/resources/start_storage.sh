#!/bin/sh

cd /radameon/web_server/
bazel run //web_server:webserver -- --port 8080 --share /exchange/storage/ &