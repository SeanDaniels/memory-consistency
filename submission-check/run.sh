#!/usr/bin/env bash
set -euo pipefail

make clean
make
./sim_program2 trace_one_critical_section.small.txt PC
