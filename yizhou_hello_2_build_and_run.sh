#!/bin/bash
# orocos_toolchain_dir=$1

# if [ -z "$orocos_toolchain_dir" ]; then
#     echo "Usage: $1 <orocos_toolchain_dir>"
#     exit 1
# fi
set -e

# . ${orocos_toolchain_dir}/build/install/env.sh
if [ -z "$RTT_COMPONENT_PATH" ]; then
    echo "RTT_COMPONENT_PATH is not set, source <orocos_toolchain>/build/install/env.sh first"
    exit 1
fi
export RTT_COMPONENT_PATH=$(pwd)/../install/lib/orocos:$(pwd)/../install/lib/orocos/gnulinux/plugins:$RTT_COMPONENT_PATH

cd rtt-exercises/hello_2_properties

mkdir -p build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$(pwd)/../../install
# make install VERBOSE=1
make install
cd ..

# in hello_2_properties
export RTT_COMPONENT_PATH=$(pwd)/../install/lib/orocos:$(pwd)/../install/lib/orocos/gnulinux/plugins:$RTT_COMPONENT_PATH
deployer-gnulinux -s start.ops