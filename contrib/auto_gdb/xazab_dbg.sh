#!/bin/bash
# use testnet settings,  if you need mainnet,  use ~/.xazabcore/xazabd.pid file instead
xazab_pid=$(<~/.xazabcore/testnet3/xazabd.pid)
sudo gdb -batch -ex "source debug.gdb" xazabd ${xazab_pid}
