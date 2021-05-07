FROM debian:stretch
LABEL maintainer="Xazab Developers <dev@xazab.xyz>"
LABEL description="Dockerised XazabCore, built from Travis"

RUN apt-get update && apt-get -y upgrade && apt-get clean && rm -fr /var/cache/apt/*

COPY bin/* /usr/bin/
