#!/bin/bash

./_autosetup
./configure --prefix=$HOME/boinc --exec-prefix=$HOME/boinc --disable-client --disable-manager
