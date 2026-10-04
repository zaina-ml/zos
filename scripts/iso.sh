#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"

echo "Creating ISO."

cp "$ROOT/main.elf" "$ROOT/iso/boot/main.elf"

xorriso -as mkisofs \
    -R \
    -r \
    -J \
    -b boot/limine-bios-cd.bin \
    -no-emul-boot \
    -boot-load-size 4 \
    -boot-info-table \
    "$ROOT/iso" \
    -o "$ROOT/zos.iso"

echo "Created: $ROOT/zos.iso"