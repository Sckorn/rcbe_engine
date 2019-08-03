#!/bin/bash

export HUID="$(id -u)"
export HGID="$(id -g)"
export OPER_HOME="/home/oper"
export DISPLAY

xhost +local:

docker run -it --rm -e DISPLAY=unix$DISPLAY -e HUID=${HUID} -e HGID=${HGID} -e OPER_HOME=${OPER_HOME} -v /tmp/.X11-unix:/tmp/.X11-unix -v $(pwd)/../../:/src/ -v /dev/snd:/dev/snd --privileged --network host --entrypoint /entrypoint.sh $1 /bin/bash

