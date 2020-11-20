#!/usr/bin/env bash
set -euo pipefail

make clean
make
./sim_program2 file1.txt PC > ../output/output.txt
diff ../output/output.txt ../validation_runs/small/validation_PC_one_critical_section.small.txt > ../output/diff.txt
