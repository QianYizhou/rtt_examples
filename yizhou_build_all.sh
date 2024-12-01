#!/bin/bash
# orocos_toolchain_dir=$1

# if [ -z "$orocos_toolchain_dir" ]; then
#     echo "Usage: $1 <orocos_toolchain_dir>"
#     exit 1
# fi
echo "param 0: $0"
echo "param 1: $1"

cleanup=0
if [ "$1" == "clean" ]; then
    cleanup=1
fi

# Exit when error occurs
set -e

# . ${orocos_toolchain_dir}/build/install/env.sh
if [ -z "$RTT_COMPONENT_PATH" ]; then
    echo "RTT_COMPONENT_PATH is not set, source <orocos_toolchain>/build/install/env.sh first"
    exit 1
fi

cur_path=$(pwd)

if [ $cleanup -eq 1 ]; then
  echo "Clean up install/ folder first ..."
  rm -rf rtt-exercises/install
fi

# Note: controller-youbot is not included in the list due to compilation error
folders=( controller-1 hello_1_task_execution hello_2_properties hello_3_dataports hello_4_operations hello_5_services hello_6_scripting hello_7_deployment helloworld )
for f in "${folders[@]}"
do
  cd $cur_path

  echo "Confiure and build $f ..."
  # echo "Value for fruits array is: $value"

  cd rtt-exercises/$f
  echo "Current path: $(pwd)"

  if [ $cleanup -eq 1 ]; then
    echo "Clean up build/ folder ..."
    rm -rf build
  fi

  mkdir -p build
  cd build
  cmake .. -DCMAKE_INSTALL_PREFIX=$(pwd)/../../install -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
  make install
done

# # in hello_1_task_execution
# export RTT_COMPONENT_PATH=$(pwd)/../install/lib/orocos:$RTT_COMPONENT_PATH
# deployer-gnulinux -s start.ops