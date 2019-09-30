#!/bin/bash

export HUID="$(id -u)"
export HGID="$(id -g)"
export OPER_HOME="/home/oper"
export DISPLAY


SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
  DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )"
  SOURCE="$(readlink "$SOURCE")"
  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )"

xhost +local:

docker run -it --rm -e DISPLAY=unix$DISPLAY -e HUID=${HUID} -e HGID=${HGID} -e OPER_HOME=${OPER_HOME} -v ${DIR}/../../../rcbe_static_storage:/home/oper/dev/rcbe_static_storage  -v /home/danny/.ssh:/home/oper/.ssh -v /tmp/.X11-unix:/tmp/.X11-unix -v ${DIR}/../../:/rcbe_engine/ -v /dev/snd:/dev/snd -v ${DIR}/../../../../bazel/cache/:/bazel/cache/ --privileged --network host --entrypoint /entrypoint.sh $1 /bin/bash

