#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "snesc.h"
#include "graph.h"
#include "input.h"

extern unsigned char tiles1[];
extern unsigned char tiles2[];
extern unsigned char bg1map[];
extern unsigned char bg2map[];
extern unsigned char palette[];
extern unsigned char backpal[];

unsigned int blockmap[0x400];
//unsigned int backmap[0x400];
unsigned int pal[0x100];
//unsigned char blocks[0x64];

static const char* LINE_BLANK = "                              ";

typedef enum { false, true } boolean;

unsigned int posTextInit = 0x080;

void capture_inputs()
{
  unsigned int posText = posTextInit;

  int i;
  for(i = 0; i < 4; i++) // super multitap not handled for now
  {
    boolean joypadHasInput = false;

    if(getjoystatus(i) & LEFT_BUTTON)
    {
      writestring("LEFT", blockmap, posText + 0x003, 0x3F6);
      setmap(0, (unsigned char*)blockmap);
      joypadHasInput = true;
    }
    if(getjoystatus(i) & RIGHT_BUTTON)
    {
      writestring("RIGHT", blockmap, posText + 0x008, 0x3F6);
      setmap(0, (unsigned char*)blockmap);
      joypadHasInput = true;
    }

    if(getjoystatus(i) & UP_BUTTON)
    {
      writestring("UP", blockmap, posText + 0x00E, 0x3F6);
      setmap(0, (unsigned char*)blockmap);
      joypadHasInput = true;
    }
    if(getjoystatus(i) & DOWN_BUTTON)
    {
      writestring("DOWN", blockmap, posText + 0x011, 0x3F6);
      setmap(0, (unsigned char*)blockmap);
      joypadHasInput = true;
    }

    if(getjoystatus(i) & A_BUTTON)
    {
      writestring("A", blockmap, posText + 0x016, 0x3F6);
      setmap(0, (unsigned char*)blockmap);
      joypadHasInput = true;
    }

    if(getjoystatus(i) & B_BUTTON)
    {
      writestring("B", blockmap, posText + 0x018, 0x3F6);
      setmap(0, (unsigned char*)blockmap);
      joypadHasInput = true;
    }

    if(getjoystatus(i) & X_BUTTON)
    {
      writestring("X", blockmap, posText + 0x01A, 0x3F6);
      setmap(0, (unsigned char*)blockmap);
      joypadHasInput = true;
    }

    if(getjoystatus(i) & Y_BUTTON)
    {
      writestring("Y", blockmap, posText + 0x01C, 0x3F6);
      setmap(0, (unsigned char*)blockmap);
      joypadHasInput = true;
    }

    if(getjoystatus(i) & START_BUTTON)
    {
      writestring("START", blockmap, posText + 0x020 + 0x003, 0x3F6);
      setmap(0, (unsigned char*)blockmap);
      joypadHasInput = true;
    }

    if(getjoystatus(i) & SELECT_BUTTON)
    {
      writestring("SELECT", blockmap, posText + 0x020 + 0x009, 0x3F6);
      setmap(0, (unsigned char*)blockmap);
      joypadHasInput = true;
    }

    if(getjoystatus(i) & TL_BUTTON)
    {
      writestring("LB", blockmap, posText + 0x020 + 0x010, 0x3F6);
      setmap(0, (unsigned char*)blockmap);
      joypadHasInput = true;
    }

    if(getjoystatus(i) & TR_BUTTON)
    {
      writestring("RB", blockmap, posText + 0x020 + 0x013, 0x3F6);
      setmap(0, (unsigned char*)blockmap);
      joypadHasInput = true;
    }

    if (joypadHasInput)
    {
      // get joypad number and status
      char num[1];
      char status[8];

      sprintf(num, "%d", i + 1);
      writestring("P", blockmap, posText, 0x3F6);
      writestring(num, blockmap, posText + 1, 0x3F6);
      setmap(0, (unsigned char*)blockmap);

      sprintf(status, "%d", getjoystatus(i));
      writestring(status, blockmap, posText + 0x040 + 0x003, 0x3F6);
      setmap(0, (unsigned char*)blockmap);
    }
    else
    {
      writestring(LINE_BLANK, blockmap, posText, 0x3F6);
      writestring(LINE_BLANK, blockmap, posText + 0x020, 0x3F6);
      writestring(LINE_BLANK, blockmap, posText + 0x040, 0x3F6);
      setmap(0, (unsigned char*)blockmap);
    }
    clearjoy(i);

    // display next joypad inputs
    posText += 0x080;
  }
}

void idle()
{
  resettimer();
  capture_inputs();
  sync(1);
}

int main()
{
  snesc_init();

  settiles(0, tiles1, 0xF00);
  memcpy(pal, palette, 0x200);

  // header
  writestring("*** JOYPAD TEST ***", blockmap, 0x020 + 0x006, 0x3F6);

  // footer
  writestring("by Modrigue", blockmap, 0x300, 0x3F6);
  writestring("Made with Optixx\'s SNES SDK,", blockmap, 0x320, 0x3F6);
  writestring("forked by Ace17.", blockmap, 0x340, 0x3F6);
  writestring("Visit github.com/Ace17/snes-sdk", blockmap, 0x360, 0x3F6);
  setpalette((unsigned char*)pal);

  enablescreen();

  setmap(0, (unsigned char*)blockmap);

  while(1)
    idle();
}
