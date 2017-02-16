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

#include <usb.h>

#include "dcdc-nuc.h"

int dcdc_get_io_data(struct usb_dev_handle *h, unsigned char *buf, int buflen) {
  unsigned char c[2];
  int ret = 0;

  if (buflen < MAX_TRANSFER_SIZE) {
    return -1;
  }

  c[0] = NUC_IN_REPORT_IO_DATA;
  c[1] = 0;

  if (dcdc_send(h, c, 2) < 0) {
    fprintf(stderr, "Cannot send command to device\n");
    return -2;
  }

  if ((ret = dcdc_recv(h, buf, MAX_TRANSFER_SIZE, 1000)) < 0) {
    fprintf(stderr, "Cannot get device status\n");
    return -3;
  }

  return ret;
}

int dcdc_get_io_data2(struct usb_dev_handle *h,
  unsigned char *buf, int buflen) {
  unsigned char c[2];
  int ret = 0;

  if (buflen < MAX_TRANSFER_SIZE) {
    return -1;
  }

  c[0] = NUC_IN_REPORT_IO_DATA2;
  c[1] = 0;

  if (dcdc_send(h, c, 2) < 0) {
    fprintf(stderr, "Cannot send command to device\n");
    return -2;
  }

  if ((ret = dcdc_recv(h, buf, MAX_TRANSFER_SIZE, 1000)) < 0) {
    fprintf(stderr, "Cannot get device status\n");
    return -3;
  }

  return ret;
}


int dcdc_parse_data(unsigned char *data, int size) {
  if (data == NULL) {
    return -1;
  }

  if (size < 24) {
    fprintf(stderr, "Data size error! Size: %d\n", size);
    return -2;
  }

#ifdef DEBUG
  int i;
  for (i = 0; i < size; i++) {
    if (i % 8 == 0) fprintf(stderr, "\n");
    fprintf(stderr, "[%02d] = 0x%02x ", i, data[i]);
  }
  fprintf(stderr, "\n");
#endif

  switch (data[0]) {
    case NUC_IN_REPORT_IO_DATA:
      dcdc_parse_io_data(data);
      break;
    case NUC_IN_REPORT_IO_DATA2:
      dcdc_parse_io_data2(data);
      break;
    default:
      fprintf(stderr, "Unknown message\n");
  }

  return 0;
}
