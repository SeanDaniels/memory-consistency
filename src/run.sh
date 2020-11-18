#!/usr/bin/env bash
set -euo pipefail

make clean
make
./sim_program2 file1.txt PC > output.txt
