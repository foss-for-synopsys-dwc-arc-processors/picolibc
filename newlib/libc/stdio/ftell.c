/*
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * and/or other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/*
FUNCTION
<<ftell>>, <<ftello>>---return position in a stream or file

INDEX
	ftell
INDEX
	ftello
INDEX
	_ftell_r
INDEX
	_ftello_r

SYNOPSIS
	#include <stdio.h>
	long ftell(FILE *<[fp]>);
	off_t ftello(FILE *<[fp]>);
	long ftell( FILE *<[fp]>);
	off_t ftello( FILE *<[fp]>);

DESCRIPTION
Objects of type <<FILE>> can have a ``position'' that records how much
of the file your program has already read.  Many of the <<stdio>> functions
depend on this position, and many change it as a side effect.

The result of <<ftell>>/<<ftello>> is the current position for a file
identified by <[fp]>.  If you record this result, you can later
use it with <<fseek>>/<<fseeko>> to return the file to this
position.  The difference between <<ftell>> and <<ftello>> is that
<<ftell>> returns <<long>> and <<ftello>> returns <<off_t>>.

In the current implementation, <<ftell>>/<<ftello>> simply uses a character
count to represent the file position; this is the same number that
would be recorded by <<fgetpos>>.

RETURNS
<<ftell>>/<<ftello>> return the file position, if possible.  If they cannot do
this, they return <<-1L>>.  Failure occurs on streams that do not support
positioning; the global <<errno>> indicates this condition with the
value <<ESPIPE>>.

PORTABILITY
<<ftell>> is required by the ANSI C standard, but the meaning of its
result (when successful) is not specified beyond requiring that it be
acceptable as an argument to <<fseek>>.  In particular, other
conforming C implementations may return a different result from
<<ftell>> than what <<fgetpos>> records.

<<ftello>> is defined by the Single Unix specification.

No supporting OS subroutines are required.
*/

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "%W% (Berkeley) %G%";
#endif /* LIBC_SCCS and not lint */

/*
 * ftell: return current offset.
 */

#define _DEFAULT_SOURCE
#include <stdio.h>
#include <errno.h>
#include "local.h"

long
ftell (
       register FILE * fp)
{
  _fpos_t pos;

  pos = ftello ( fp);
  if ((long)pos != pos)
    {
      pos = -1;
      errno = EOVERFLOW;
    }
  return (long)pos;
}
