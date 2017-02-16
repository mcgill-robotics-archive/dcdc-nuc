/*
 * Copyright (c) 2011 by Mini-Box.com, iTuner Networks Inc.
 * Written by Nicu Pavel <npavel@mini-box.com>
 * All Rights Reserved
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#include "dcdc-nuc.h"

#define P(t, v...) fprintf(stderr, t "\n", ##v)

unsigned int g_memTerm[TERMISTOR_CONSTS] = {
  (unsigned int) 0xB,
  (unsigned int) 0xE,
  (unsigned int) 0x13,
  (unsigned int) 0x19,
  (unsigned int) 0x1F,
  (unsigned int) 0x28,
  (unsigned int) 0x32,
  (unsigned int) 0x3E,
  (unsigned int) 0x4C,
  (unsigned int) 0x5D,
  (unsigned int) 0x6F,
  (unsigned int) 0x85,
  (unsigned int) 0x9D,
  (unsigned int) 0xB8,
  (unsigned int) 0xD6,
  (unsigned int) 0xF6,
  (unsigned int) 0x118,
  (unsigned int) 0x13C,
  (unsigned int) 0x162,
  (unsigned int) 0x188,
  (unsigned int) 0x1B0,
  (unsigned int) 0x1D6,
  (unsigned int) 0x1FC,
  (unsigned int) 0x222,
  (unsigned int) 0x246,
  (unsigned int) 0x268,
  (unsigned int) 0x289,
  (unsigned int) 0x2A8,
  (unsigned int) 0x2C5,
  (unsigned int) 0x2E0,
  (unsigned int) 0x2F9,
  (unsigned int) 0x310,
  (unsigned int) 0x325,
  (unsigned int) 0x339
};

float therm2temp(unsigned int termistor) {
  float value;
  if (termistor <= g_memTerm[0]) {
    value = -40;
  } else if (termistor >= g_memTerm[TERMISTOR_CONSTS-1]) {
    value = 125;
  } else {
    int pos = -1;
    for (int i = TERMISTOR_CONSTS-1; i >= 0; i--) {
      if (termistor >= g_memTerm[i]) {
        pos = i;
        break;
      }
    }

    if (termistor == g_memTerm[pos]) {
      value = pos * 5 - 40;
    } else {
      int t1 = pos * 5 - 40;
      int t2 = (pos + 1) * 5 - 40;

      unsigned int d1 = g_memTerm[pos];
      unsigned int d2 = g_memTerm[pos + 1];

      float dtemp = ((float)termistor -
        (float)d1) * ((float)t2 - (float)t1) / ((float)d2 - (float)d1);

      int temp = (int)ceil(dtemp) + t1;

      value = temp;
    }
  }
  return value;
}

void dcdc_parse_io_data(unsigned char *data) {
  unsigned int i, input_flag, output_flag;
  float input_voltage, input_current, output_voltage, output_current,
    temperature, ignition_voltage, output_power, thump_voltage;

  input_flag = (((unsigned int)data[1]) << 8) | data[2];
  output_flag = (((unsigned int)data[3]) << 8) | data[4];

  i = (((unsigned int)data[5]) << 8) | data[6];
  input_voltage = (float)i / 1000;

  i = (((unsigned int)data[7]) << 8) | data[8];
  input_current = (float)i / 1000;

  i = (((unsigned int)data[9]) << 8) | data[10];
  output_voltage = (float)i / 1000;

  i = (((unsigned int)data[11]) << 8) | data[12];
  output_current = (float)i / 1000;

  if (output_flag & 0x0100) {  // ENOUT
    i = (data[13] << 8) | data[14];
    temperature = therm2temp(i);
  } else {
    temperature = 1000.0;
  }

  i = (((unsigned int)data[15]) << 8) | data[16];
  ignition_voltage = (float)i / 1000;

  i =  (((unsigned int)data[17]) << 24)
    |(((unsigned int)data[18]) << 16)
    |(((unsigned int)data[19]) << 8)
    | data[20];
  output_power = (float)i / 1000;

  i = (((unsigned int)data[21]) << 8) | data[22];
  thump_voltage = (float)i / 1000;

  P("Protection ok: %s", ((input_flag & 0x0001) ? "True":"False"));
  P("nProtection fault: %s", ((input_flag & 0x0002) ? "True":"False"));
  P("nOpen LED: %s", ((input_flag & 0x04) ? "True":"False"));
  P("nShort LED: %s", ((input_flag & 0x08) ? "True":"False"));
  // P("---: %s", ((input_flag & 0x0010) ? "True":"False"));
  // P("---: %s", ((input_flag & 0x0020) ? "True":"False"));
  // P("---: %s", ((input_flag & 0x0040) ? "True":"False"));
  // P("---: %s", ((input_flag & 0x0080) ? "True":"False"));
  P("CFG1: %s", ((input_flag & 0x0100) ? "True":"False"));
  P("CFG2: %s", ((input_flag & 0x0200) ? "True":"False"));
  P("CFG3: %s", ((input_flag & 0x0400) ? "True":"False"));
  P("Mode: %s", ((input_flag & 0x0800) ? "True":"False"));
  P("USB Sense: %s", ((input_flag & 0x1000) ? "True":"False"));
  // P("---: %s", ((input_flag & 0x2000) ? "True":"False"));
  // P("---: %s", ((input_flag & 0x4000) ? "True":"False"));
  // P("---: %s", ((input_flag & 0x8000) ? "True":"False"));

  P("VIN_GOOD: %s", ((output_flag & 0x0001) ? "True":"False"));
  P("IGN_GOOD : %s", ((output_flag & 0x0002) ? "True":"False"));
  P("MOB_ALIVE_POUT: %s", ((output_flag & 0x04) ? "True":"False"));
  // P("---: %s", ((output_flag & 0x08) ? "True":"False"));
  // P("---: %s", ((output_flag & 0x0010) ? "True":"False"));
  // P("---: %s", ((output_flag & 0x0020) ? "True":"False"));
  P("IGN_RISED: %s", ((output_flag & 0x0040) ? "True":"False"));
  P("IGN_FALLED: %s", ((output_flag & 0x0080) ? "True":"False"));
  P("ENOUT: %s", ((output_flag & 0x0100) ? "True":"False"));
  P("Thump: %s", ((output_flag & 0x0200) ? "True":"False"));
  P("ctrlFREQ: %s", ((output_flag & 0x0400) ? "True":"False"));
  P("nPSW: %s", ((output_flag & 0x0800) ? "True":"False"));
  // P("---: %s", ((output_flag & 0x1000) ? "True":"False"));
  // P("---: %s", ((output_flag & 0x2000) ? "True":"False"));
  // P("---: %s", ((output_flag & 0x4000) ? "True":"False"));
  // P("---: %s", ((output_flag & 0x8000) ? "True":"False"));

  P("input voltage: %.2f", input_voltage);
  P("input current: %.2f", input_current);
  P("output voltage: %.2f", output_voltage);
  P("output current: %.2f", output_current);
  P("temperature: %.2f", temperature);
  P("ignition voltage: %.2f", ignition_voltage);
  P("output power: %2f", output_power);
  P("thump voltage: %2f", thump_voltage);
}

void dcdc_parse_io_data2(unsigned char *data) {
  unsigned int timer_init, timer_ign2out_on, timer_thump, timer_out2mb_pulse,
    timer_mb_pulse_width, timer_ign_cancel, timer_ign_off2mb_pulse_off,
    timer_hard_off, timer_vin_cnt, timer_iin_cnt, state_machine, mode,
    version_major, version_minor;

  timer_init = ((((unsigned int)data[1]) << 8) | data[2]) * 10;
  timer_ign2out_on = ((((unsigned int)data[3]) << 8) | data[4]) * 10;
  timer_thump = (((unsigned int)data[5]) << 8) | data[6];
  timer_out2mb_pulse = (((unsigned int)data[7]) << 8) | data[8];
  timer_mb_pulse_width = ((((unsigned int)data[9]) << 8) | data[10]) * 10;
  timer_ign_cancel = (((unsigned int)data[11]) << 8) | data[12];
  timer_ign_off2mb_pulse_off = (((unsigned int)data[13]) << 8) | data[14];
  timer_hard_off = (((unsigned int)data[15]) << 8) | data[16];
  timer_vin_cnt = ((((unsigned int)data[17]) << 8) | data[18]) * 10;
  timer_iin_cnt = ((((unsigned int)data[19]) << 8) | data[20]) * 10;

  state_machine = (unsigned int)data[21];
  mode = data[22];

  version_major = (data[25] >> 4) & 0x0F;
  version_minor = data[25] & 0x0F;

  P("init timer(ms): %d", timer_init);
  P("ignition on to ouput on(s): %d", timer_ign2out_on);
  P("thump timer(s): %d", timer_thump);
  P("ouput on to motherboard pulse on(ms): %d", timer_out2mb_pulse);
  P("motherboad pulse width(ms): %d", timer_mb_pulse_width);
  P("ignition cancel(s): %d", timer_ign_cancel);
  P("ignition off to motherboad pulse off(s): %d", timer_ign_off2mb_pulse_off);
  P("hard off timer(s): %d", timer_hard_off);
  P("input voltage count(ms): %d", timer_vin_cnt);
  P("input current count(ms): %d", timer_iin_cnt);
  P("state machine state: %d", state_machine);
  P("mode: %s", mode ? "automotive" : "dumb");
  P("firmware version: %d.%d", version_major, version_minor);
}

void dcdc_parse_internal_msg(unsigned char *data) {
    P("Parsing INTERNAL MESSAGE: Not implemented");
}

void dcdc_parse_mem(unsigned char *data) {
    P("Parsing MEM READ IN: Not implemented");
}
