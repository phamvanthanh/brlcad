/*
 *			A S I Z E . C
 *
 * Image file AutoSizing code.
 *
 *  Currently #included by bw-fb, pix-fb, and others.
 *  Might want to go into a library (libfb?)
 *
 *  Author -
 *	Phil Dykstra
 *  
 *  Source -
 *	SECAD/VLD Computing Consortium, Bldg 394
 *	The U. S. Army Ballistic Research Laboratory
 *	Aberdeen Proving Ground, Maryland  21005-5066
 *  
 *  Distribution Status -
 *	Public Domain, Distribution Unlimitied.
 */

#ifndef lint
static char RCSid[] = "@(#)$Header$ (BRL)";
#endif

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

struct sizes {
	int	width;		/* image width in pixels */
	int	height;		/* image height in pixels */
	long	size;		/* total number of pixels */
};
struct sizes fb_common_sizes[] = {
	{   50,	  50,	    2500 },
	{   64,	  64,	    4096 },
	{  128,	 128,	   16384 },
	{  160,  120,	   19200 },	/* quarter 640x480 */
	{  256,	 256,	   65536 },
	{  320,  240,	   76800 },	/* half 640x480 */
	{  512,	 512,	  262144 },
	{  640,	 480,	  307200 },	/* Common video format */
	{  720,	 486,	  349920 },	/* Abekas video format */
	{ 1024,	 768,	  786432 },	/* SGI-3D screen size */
	{ 1152,  900,	 1036800 },	/* Sun screen size */
	{ 1024,	1024,	 1048576 },
	{ 1280,  960,	 1228800 },	/* twice 640x480 */
	{ 1280,	1024,	 1310720 },	/* SGI-4D screen size */
	{ 2048, 2048,	 4194304 },
	{ 4096, 4096,	16777216},
	{ 8192, 8192,	67108864},
	{    0,	   0,	0 }
};

/*
 *			F B _ C O M M O N _ F I L E _ S I Z E
 *
 *  Returns non-zero if it finds a matching size
 */
int
fb_common_file_size( widthp, heightp, filename, pixel_size )
int	*widthp;		/* pointer to returned width */
int	*heightp;		/* pointer to returned height */
char	*filename;		/* image file to stat */
int	pixel_size;		/* bytes per pixel */
{
	struct	stat	sbuf;
	int	size;

	if( filename == NULL || *filename == NULL )
		return	0;
	if( stat( filename, &sbuf ) < 0 )
		return	0;

	size = sbuf.st_size / pixel_size;

	return fb_common_image_size( widthp, heightp, size );
}

/*
 *			F B _ C O M M O N _ I M A G E _ S I Z E
 *
 *  Given the number of pixels in an image file,
 *  if this is a "common" image size,
 *  return the width and height of the image.
 *
 *  Returns -
 *	0	size unknown
 *	1	width and height returned
 */
int
fb_common_image_size( widthp, heightp, npixels )
int	*widthp;		/* pointer to returned width */
int	*heightp;		/* pointer to returned height */
register int	npixels;	/* Number of pixels */
{
	register struct	sizes	*sp;

	if( npixels <= 0 )
		return	0;

	sp = fb_common_sizes;
	while( sp->size != 0 ) {
		if( sp->size == npixels ) {
			*widthp = sp->width;
			*heightp = sp->height;
			return	1;
		}
		sp++;
	}
	return	0;
}
