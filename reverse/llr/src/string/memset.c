/* Copyright (C) 1991-2015 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <common.h>

void *
memset (dstpp, c, len)
     void *dstpp;
     int c;
     size_t len;
{
  long int dstp = (long int) dstpp;

  if (len >= 8)
    {
      size_t xlen;
      unsigned cccc;

      cccc = (unsigned char) c;
      cccc |= cccc << 8;
      cccc |= cccc << 16;

      /* There are at least some bytes to set.
	 No need to test for LEN == 0 in this alignment loop.  */
      while (dstp % 4 != 0)
	{
	  ((char *) dstp)[0] = c;
	  dstp += 1;
	  len -= 1;
	}

      /* Write 8 `op_t' per iteration until less than 8 `op_t' remain.  */
      xlen = len / 32;
      while (xlen > 0)
	{
	  ((unsigned *) dstp)[0] = cccc;
	  ((unsigned *) dstp)[1] = cccc;
	  ((unsigned *) dstp)[2] = cccc;
	  ((unsigned *) dstp)[3] = cccc;
	  ((unsigned *) dstp)[4] = cccc;
	  ((unsigned *) dstp)[5] = cccc;
	  ((unsigned *) dstp)[6] = cccc;
	  ((unsigned *) dstp)[7] = cccc;
	  dstp += 32;
	  xlen -= 1;
	}
      len %= 32;

      /* Write 1 `op_t' per iteration until less than OPSIZ bytes remain.  */
      xlen = len / 4;
      while (xlen > 0)
	{
	  ((unsigned *) dstp)[0] = cccc;
	  dstp += 4;
	  xlen -= 1;
	}
      len %= 4;
    }

  /* Write the last few bytes.  */
  while (len > 0)
    {
      ((char *) dstp)[0] = c;
      dstp += 1;
      len -= 1;
    }

  return dstpp;
}
