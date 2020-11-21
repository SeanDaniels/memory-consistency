#!/usr/bin/env bash
set -euo pipefail

make clean
make
./sim_program2 trace_one_critical_section.small.txt RC
./sim_program2 trace_one_critical_section.small.txt PC
./sim_program2 trace_one_critical_section.small.txt SC
./sim_program2 trace_one_critical_section.small.txt WO
