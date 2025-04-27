#!/bin/bash

example_dir=$(dirname $(realpath 0))
proj_dir=$(cd  ${example_dir}/../.. ; pwd)

vlib work
vlog -sv +incdir+${proj_dir}/src/include/vsc/solvers/sv \
    ${proj_dir}/src/include/vsc/solvers/sv/vsc_solvers.sv \
    top.sv

vopt -o top_opt top

vsim -c -do "run -a; quit -f" top_opt


