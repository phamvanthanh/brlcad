/*
	Author:		Gary S. Moss
			U. S. Army Ballistic Research Laboratory
			Aberdeen Proving Ground
			Maryland 21005-5066
			(301)278-6647 or AV-298-6647
*/
#ifndef lint
static char RCSid[] = "@(#)$Header$ (BRL)";
#endif

#include <stdio.h>
#include <fcntl.h>
#include "machine.h"
#include "vmath.h"
#include "fb.h"
#include "./lgt.h"
#include "./screen.h"
#include "./extern.h"
int		zoom;	/* Current zoom factor of frame buffer.		*/
int		fb_Setup();
void		fb_Zoom_Window();

/*	f b _ S e t u p ( )						*/
int
fb_Setup( file, size )
char	*file;
int	size;
	{
#ifdef sgi
		static int	sgi_open = FALSE;
		static FBIO	*sgi_iop;
	if( sgi_open )
		{
		if( file[0] == '\0' || strncmp( file, "/dev/sgi", 8 ) == 0 )
			{
			fbiop = sgi_iop;
			return	1; /* Only open SGI once.		*/
			}
		}
#endif
	if( tty )
		prnt_Event( "Opening device..." );
	size = size > 512 ? 1024 : 512;
	if(	(fbiop = fb_open(	file[0] == '\0' ? NULL : file,
					size, size
					)
		) == FBIO_NULL
	    ||	fb_ioinit( fbiop ) == -1
	    ||	fb_wmap( fbiop, COLORMAP_NULL ) == -1
		)
		return	0;
	(void) fb_setcursor( fbiop, arrowcursor, 16, 16, 0, 0 );
	(void) fb_cursor( fbiop, 1, size/2, size/2 );
	if( tty )
		prnt_Event( (char *) NULL );
#ifdef sgi
	if( strncmp( fbiop->if_name, "/dev/sgi", 8 ) == 0 )
		{
		sgi_open = TRUE;
		sgi_iop = fbiop;
		}
#endif
	return	1;
	}

/*	f b _ Z o o m _ W i n d o w ( )					*/
void
fb_Zoom_Window()
	{	register int	xpos, ypos;
	zoom = fb_getwidth( fbiop ) / grid_sz;
	xpos = ypos = grid_sz / 2;
	if( tty )
		prnt_Event( "Zooming..." );
	if( fb_zoom( fbiop, zoom, zoom ) == -1 )
		rt_log( "Can not set zoom <%d,%d>.\n", zoom, zoom );
	if( x_fb_origin >= grid_sz )
		xpos += x_fb_origin;
	if( y_fb_origin >= grid_sz )
		ypos += y_fb_origin;
	if( tty )
		prnt_Event( "Windowing..." );
	if( fb_viewport( fbiop, 0, 0, grid_sz, grid_sz ) == -1 )
		rt_log( "Can not set viewport {<%d,%d>,<%d,%d>}.\n",
			0, 0, grid_sz, grid_sz
			);
	if( fb_window( fbiop, xpos, ypos ) == -1 )
		rt_log( "Can not set window <%d,%d>.\n", xpos, ypos );
	if( tty )
		prnt_Event( (char *) NULL );
	return;
	}
