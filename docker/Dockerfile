FROM phusion/baseimage:bionic-1.0.0
LABEL maintainer="Xazab Developers <dev@xazab.org>"
LABEL description="Dockerised XazabCore, built from Travis"

ARG USER_ID
ARG GROUP_ID

ENV HOME /xazab

# add user with specified (or default) user/group ids
ENV USER_ID ${USER_ID:-1000}
ENV GROUP_ID ${GROUP_ID:-1000}
RUN groupadd -g ${GROUP_ID} xazab
RUN useradd -u ${USER_ID} -g xazab -s /bin/bash -m -d /xazab xazab
RUN mkdir /xazab/.xazabcore
RUN chown xazab:xazab -R /xazab

COPY bin/* /usr/local/bin/

RUN chmod a+x /usr/local/bin/*

USER xazab

VOLUME ["/xazab"]

EXPOSE 9998 9999 19998 19999

WORKDIR /xazab
