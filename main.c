#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
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

unsigned int posTextInit = 0x080;

void display_string(const char* s, int pos)
{
  writestring(s, blockmap, pos, 0x3F6);
}

void capture_inputs()
{
  unsigned int posText = posTextInit;

  int i;
  for(i = 0; i < 4; i++) // super multitap not handled for now
  {
    const int status = getjoystatus(i);

    if(status & LEFT_BUTTON)
    {
      display_string("LEFT", posText + 0x003);
    }
    if(status & RIGHT_BUTTON)
    {
      display_string("RIGHT", posText + 0x008);
    }

    if(status & UP_BUTTON)
    {
      display_string("UP", posText + 0x00E);
    }
    if(status & DOWN_BUTTON)
    {
      display_string("DOWN", posText + 0x011);
    }

    if(status & A_BUTTON)
    {
      display_string("A", posText + 0x016);
    }

    if(status & B_BUTTON)
    {
      display_string("B", posText + 0x018);
    }

    if(status & X_BUTTON)
    {
      display_string("X", posText + 0x01A);
    }

    if(status & Y_BUTTON)
    {
      display_string("Y", posText + 0x01C);
    }

    if(status & START_BUTTON)
    {
      display_string("START", posText + 0x020 + 0x003);
    }

    if(status & SELECT_BUTTON)
    {
      display_string("SELECT", posText + 0x020 + 0x009);
    }

    if(status & TL_BUTTON)
    {
      display_string("LB", posText + 0x020 + 0x010);
    }

    if(status & TR_BUTTON)
    {
      display_string("RB", posText + 0x020 + 0x013);
    }

    if (status)
    {
      // get joypad number and status
      char num[1];
      char status[8];

      sprintf(num, "%i", i + 1);
      writestring("P", blockmap, posText, 0x3F6);
      writestring(num, blockmap, posText + 1, 0x3F6);

      sprintf(status, "%X", status);
      writestring(status, blockmap, posText + 0x040 + 0x003, 0x3F6);
    }
    else
    {
      writestring(LINE_BLANK, blockmap, posText, 0x3F6);
      writestring(LINE_BLANK, blockmap, posText + 0x020, 0x3F6);
      writestring(LINE_BLANK, blockmap, posText + 0x040, 0x3F6);
    }
    clearjoy(i);

    // display next joypad inputs
    posText += 0x080;
  }

  setmap(0, (unsigned char*)blockmap);
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
  writestring("*** JOYPAD TEST v0.2 ***", blockmap, 0x020 + 0x004, 0x3F6);

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
