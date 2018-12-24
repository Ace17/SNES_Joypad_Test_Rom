# SNES_Joypad_Test_Rom
Use this simple SNES Joypad test rom to check if your controllers are correctly configured.
The code is made with Optixx's SNES SDK, forked by Ace17.

The ROM compiles as follows:

- First visit and clone https://github.com/Ace17/snes-sdk.git,
- Follow the compilation and installation procedure,
- Create a new directory and clone https://github.com/Modrigue/SNES_Joypad_Test_Rom.git,
- make

The compiled ROM is in the bin directory.

Notes:
- BUG: Joypad 4 not detected for now. Joypad 3 detected as Joypad 4.
- Super multitap (5 controllers) is not handled yet.
