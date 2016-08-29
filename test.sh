#!/bin/bash

(cd libecho && make) &&
    printf '\x00\x00\x00\x04echo\x00\x00\x00\x05hello' \
        | ./so_host libecho/libecho.so \
        | hexdump -C
