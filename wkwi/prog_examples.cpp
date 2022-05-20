// -----------------------------------------------------------------------------
// Altair 8800 Simulator
// Copyright (C) 2017 David Hansel
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
// -----------------------------------------------------------------------------

#include <Arduino.h>
#include "config.h"
#include "prog_basic.h"
#include "mem.h"

#if defined(__AVR_ATmega2560__)

#include "prog_examples_basic_mega.h"

// not enough room in MEGA to store ASM examples (or PS2 assembler anyways)
const char * const asm_programs[] = {};
byte read_asm_example(int n, int i) { return 0; }

#define NUM_ASM_PROGRAMS 0

#else

#include "prog_examples_basic_due.h"
#include "prog_examples_asm.h"

#endif

static byte     prog_idx = 0;
static uint16_t prog_ctr = 0;
static byte     NULs     = 0;


bool prog_examples_read_start(byte idx)
{
  if( idx < NUM_BASIC_PROGRAMS || idx >= 0x80 && idx < 0x80+NUM_ASM_PROGRAMS || idx == 0xc0 )
    {
      prog_idx = idx;
      prog_ctr = 0;
      NULs     = 0;

      if( idx == 0xc0 )
        {
          // 4k BASIC will get into an infinite loop if a full 64k RAM are
          // available => purposely reduce the RAM size by 1 byte if necessary
          if( mem_is_writable(0x1000, 0xFFFF) ) mem_add_rom(0xFFFF, 1, "RAMLIMIT", MEM_ROM_FLAG_TEMP);
        }

      return true;
    }
  else
    return false;
}


bool prog_examples_read_next(byte dev, byte *b)
{
  if( NULs>0 )
    {
      NULs--;
      *b = 0;
      return true;
    }
  else if( prog_idx < 0x80 )
    *b = read_basic_example(prog_idx, prog_ctr);
  else if( prog_idx == 0xC0 )
    return prog_basic_read_4k(prog_ctr++, b);
  else
    *b = read_asm_example(prog_idx-0x80, prog_ctr);

  if( *b=='\r' ) NULs = config_serial_playback_example_nuls(dev);
  if( *b>0 ) prog_ctr++;
  return *b != 0;
}
