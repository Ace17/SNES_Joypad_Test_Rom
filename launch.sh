#!/usr/bin/env bash
set -euo pipefail

make -j`nproc`

options=()

# Enable both multitaps
options+=(-snes.input.port1.multitap 1)
options+=(-snes.input.port2.multitap 1)

# Map Keypad 1-8 to the 'A' button of the 8 controllers
options+=(-snes.input.port1.gamepad.a "keyboard 0x0 89")
options+=(-snes.input.port2.gamepad.a "keyboard 0x0 90")
options+=(-snes.input.port3.gamepad.a "keyboard 0x0 91")
options+=(-snes.input.port4.gamepad.a "keyboard 0x0 92")
options+=(-snes.input.port5.gamepad.a "keyboard 0x0 93")
options+=(-snes.input.port6.gamepad.a "keyboard 0x0 94")
options+=(-snes.input.port7.gamepad.a "keyboard 0x0 95")
options+=(-snes.input.port8.gamepad.a "keyboard 0x0 96")

mednafen \
  -video.fs 0 \
  "${options[@]}" \
  bin/JoypadTest.smc
