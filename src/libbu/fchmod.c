/*                        F C H M O D . C
 * BRL-CAD
 *
 * Copyright (c) 2007-2008 United States Government as represented by
 * the U.S. Army Research Laboratory.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this file; see the file named COPYING for more
 * information.
 */
/** @addtogroup libbu */
/** @{ */
/** @file fchmod.c
 *
 * @brief
 *  Wrapper around fchmod.
 *
 * @par Functions
 *	bu_fchmod  	Change file permissions
 *
 * @author Bob Parker
 *
 */

#include "common.h"

#include "machine.h"
#include "bu.h"

/* XXX 
 * For the moment we're passing filename. There should
 * be a way to get this from FILE * on Windows. A quick
 * look yielded nada.
 */
int
bu_fchmod(const char *filename,
	  FILE	     *fp,
	  int	     pmode)
{
#ifdef HAVE_FCHMOD
    if (fp) {
	return fchmod(fileno(fp), pmode);
    }
#endif    
    if (filename) {
	return chmod(filename, pmode);
    }
    return 0;
}

/** @} */
/*
 * Local Variables:
 * mode: C
 * tab-width: 8
 * indent-tabs-mode: t
 * c-file-style: "stroustrup"
 * End:
 * ex: shiftwidth=4 tabstop=8
 */
