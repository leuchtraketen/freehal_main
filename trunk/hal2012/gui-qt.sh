#!/bin/bash

cd gui-qt
qmake
make
bin/freehal >/dev/null 2>&1

