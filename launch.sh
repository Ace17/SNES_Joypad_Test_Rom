#!/usr/bin/env bash
set -euo pipefail

make -j`nproc`

mednafen \
  -video.fs 0 \
  -snes.input.port1.multitap 1 \
  -snes.input.port2.multitap 1 \
  -snes.input.port1.gamepad.a "keyboard 0x0 6" \
  -snes.input.port2.gamepad.a "keyboard 0x0 6" \
  -snes.input.port3.gamepad.a "keyboard 0x0 6" \
  -snes.input.port4.gamepad.a "keyboard 0x0 6" \
  -snes.input.port5.gamepad.a "keyboard 0x0 6" \
  -snes.input.port6.gamepad.a "keyboard 0x0 6" \
  -snes.input.port7.gamepad.a "keyboard 0x0 6" \
  -snes.input.port8.gamepad.a "keyboard 0x0 6" \
  bin/JoypadTest.smc
