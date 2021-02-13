#!/usr/bin/env bash
# use testnet settings,  if you need mainnet,  use ~/.xazabcore/xazabd.pid file instead
export LC_ALL=C

xazab_pid=$(<~/.xazabcore/testnet3/xazabd.pid)
sudo gdb -batch -ex "source debug.gdb" xazabd ${xazab_pid}
