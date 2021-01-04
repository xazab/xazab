#!/usr/bin/env bash

export LC_ALL=C

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR/.. || exit

DOCKER_IMAGE=${DOCKER_IMAGE:-xazab/xazabd-develop}
DOCKER_TAG=${DOCKER_TAG:-latest}

BUILD_DIR=${BUILD_DIR:-.}

rm docker/bin/*
mkdir docker/bin
cp $BUILD_DIR/src/xazabd docker/bin/
cp $BUILD_DIR/src/xazab-cli docker/bin/
cp $BUILD_DIR/src/xazab-tx docker/bin/
strip docker/bin/xazabd
strip docker/bin/xazab-cli
strip docker/bin/xazab-tx

docker build --pull -t $DOCKER_IMAGE:$DOCKER_TAG -f docker/Dockerfile docker
