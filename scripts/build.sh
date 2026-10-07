#!/usr/bin/env bash
set -euo pipefail

make clean
make 
make iso
./scripts/run.sh