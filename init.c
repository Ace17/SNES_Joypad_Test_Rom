#include "snesc.h"
#include "mem.h"
#include "regs.h"

extern void* __nmi_handler;

unsigned char snesc_do_copy; /* bit 7: do sprite copy, bit 6: do palette copy
                                bits 0-5: DMA transfer count */
unsigned char snesc_timer_enabled; /* each bit one timer */
unsigned int snesc_timers[16];

struct oam_table1 snesc_oam_table1[128];    /* OAM low table */
unsigned int snesc_oam_table2[16];  /* OAM high table */

unsigned char snesc_palette[512];   /* palette */

unsigned int snesc_controllers[8];  // data from controllers

struct dma_transfer snesc_dma_transfers[64]; /* DMA transfers
                                                0 (src_addr, src_bank) src address (24 bit)
                                                3 (dest) dest address (16 bit)
                                                5 (size) size (16 bit)
                                                7 (type) type (8 bit, 0 == VRAM, 1 == OAM, 2 == CGRAM, >2 == ???) */

void do_dma(unsigned char do_flags)
{
  if(do_flags & 0x80)      /* copy sprites? */
  {
    *((unsigned short*)0x4300) = 0x400; /* DMA channel 0 Bus B addr $2104 (OAM write) */
    /* the original code loads $8000, turning on sprite priority and
       messing up the gfx (some emulators don't emulate this
       correctly, which is probably why it has slipped through) */
    *(unsigned short*)0x2102 = 0;   /* OAM address 0 */
    *((void**)0x4302) = snesc_oam_table1; /* DMA source address (24 bits); hairy: writes 32 bits... */
    *(unsigned short*)0x4305 = 0x220;   /* DMA size */
    *(unsigned char*)0x420b = 1;    /* enable DMA 0 */
  }

  if(do_flags & 0x40)      /* copy palette? */
  {
    *(unsigned short*)0x4300 = 0x2200;  /* DMA channel 0 Bus B addr $2122 (CGRAM write) */
    *((void**)0x4302) = snesc_palette;  /* DMA source address; see above */
    *(unsigned short*)0x4305 = 0x200;   /* DMA size */
    *(unsigned char*)0x2121 = 0;    /* CGRAM address 0 */
    *(unsigned char*)0x420b = 1;    /* enable DMA 0 */
  }
}

u16 portData1;
u16 portData2;

void read_port(u16 reg)
{
  *((u8*)REG_JOYSER0) = 1;
  *((u8*)REG_JOYSER0) = 0;

  portData1 = 0;
  portData2 = 0;

  int i;
  for(i=0;i<16;++i)
  {
    const u8 data = ((u8*)reg)[0];

    portData1 <<= 1;
    portData1 |= (data>>0)&1;

    portData2 <<= 1;
    portData2 |= (data>>1)&1;
  }
}

void update_joypads()
{
  u16 reg[] = { 0, 0 };

  *((u8*)REG_WRIO) |= 0xc0;

  read_port(REG_JOYSER0);
  snesc_controllers[0] = portData1;
  snesc_controllers[5] = portData2;

  read_port(REG_JOYSER1);
  snesc_controllers[1] = portData1;
  snesc_controllers[2] = portData2;

  *((u8*)REG_WRIO) &= 0xc0;

  read_port(REG_JOYSER0);
  snesc_controllers[6] = portData1;
  snesc_controllers[7] = portData2;

  read_port(REG_JOYSER1);
  snesc_controllers[3] = portData1;
  snesc_controllers[4] = portData2;
}

void snesc_vblank(void)
{
  /* stuff pending for DMA? */
  unsigned char do_flags = snesc_do_copy;

  if(do_flags)
  {
    snesc_do_copy = 0;
    do_dma(do_flags);
    do_flags &= 0x3f;   /* mask out palette and sprite flags (used by do_dma) */

    if(do_flags)
    {
      int x = 0;

      while(do_flags)
      {
        switch(snesc_dma_transfers[x].src.c.type)   /* type of transfer */
        {
        case 0: /* VRAM */
          poke16(0x4300, 0x1801); // 2 regs write once, Bus B addr $2118 (VRAM data write)
          poke16(0x2116, snesc_dma_transfers[x].dest); // VRAM address
          break;
        case 1: /* OAM */
          poke16(0x4300, 0x400); // DMA OAM write
          poke16(0x2102, snesc_dma_transfers[x].dest); // OAM address
          break;
        case 2: /* CGRAM */
          poke16(0x4300, 0x2200); // CGRAM write
          poke(0x2121, snesc_dma_transfers[x].dest); // CGRAM address (trunc'd to 8 bits)
          break;
        }

        *((unsigned short*)0x4302) = snesc_dma_transfers[x].src.c.addr; /* DMA source address (16-bit) */
        *((unsigned short*)0x4305) = snesc_dma_transfers[x].size;   /* DMA size */
        *((unsigned char*)0x4304) = snesc_dma_transfers[x].src.c.bank;  /* DMA source address (bank) */
        *((unsigned char*)0x420b) = 1;  /* enable DMA 0 */
        x++;
        do_flags--;
      }
    }
  }

  update_joypads();

  /* timer ticks */
  unsigned char timers_enabled = snesc_timer_enabled;
  unsigned int tc = 0;

  while(timers_enabled & 1)
  {
    snesc_timers[tc]++;
    tc++;
    timers_enabled >>= 1;
  }
}

void snesc_init(void)
{
  int i;
  __nmi_handler = snesc_vblank; /* register vblank handler */
  *((unsigned char*)REG_NMITIMEN) = 0x80; /* enable NMI */
  snesc_timer_enabled = snesc_do_copy = snesc_controllers[0] = 0;

  /* snesc sprite init stuff */
  for(i = 0; i < 128; i++)
  {
    snesc_oam_table1[i].x = 0;
    snesc_oam_table1[i].y = 0xe8;
    snesc_oam_table1[i].t = 0;
    snesc_oam_table1[i].p = 0;
  }

  for(i = 0; i < 32; i += 2)
  {
    snesc_oam_table2[i] = 0;
  }
}

