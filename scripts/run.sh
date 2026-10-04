#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"

qemu-system-x86_64 \
    -cdrom "$ROOT/zos.iso" \
    -m 256M