#!/usr/bin/env bash
set -euo pipefail

make clean
make
echo "Release Consistency" > output.txt
./sim_program2 trace_one_critical_section.small.txt RC >> output.txt
echo "Processor Consistency" >> output.txt
./sim_program2 trace_one_critical_section.small.txt PC >> output.txt
echo "Sequential Consistency" >> output.txt
./sim_program2 trace_one_critical_section.small.txt SC >> output.txt
echo "Weak Ordering" >> output.txt
./sim_program2 trace_one_critical_section.small.txt WO >> output.txt
