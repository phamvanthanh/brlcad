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
#include "machine.h"
#include "vmath.h"
#include "raytrace.h"
#include "fb.h"
#include "./vecmath.h"
#include "./lgt.h"
#include "./tree.h"
FBIO	*fbiop = FBIO_NULL;    /* Framebuffer interface ptr.	*/

/* Initialization for root of IR data base octree.			*/
PtList	ir_ptlist = { 0.0, 0.0, 0.0, PTLIST_NULL };
Octree	ir_octree =
	{ 0, ABSOLUTE_ZERO, &ir_ptlist, TRIE_NULL, OCTREE_NULL, OCTREE_NULL };

/* Light sources.
	lgts[0]		ambient lighting
	lgts[1]		primary lighting
	...		user defined
 */
Lgt_Source	lgts[MAX_LGTS];

/* Animation control structure.						*/
Movie	movie =
	{
	FALSE,	/* m_fullscreen */
	TRUE,	/* m_lgts */
	FALSE,	/* m_over */
	FALSE,	/* m_keys */
	1,	/* m_noframes */
	0,	/* m_curframe */
	0,	/* m_endframe */
	-1,	/* m_frame_sz */
	NULL,	/* m_keys_fp */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

/* Globals for line-buffering pixel I/O.				*/
RGBpixel	bgpixel;		/* Background color.		*/

/* IR data base region trie tree root.					*/
Trie			*reg_triep = TRIE_NULL;

/* Optional files.							*/
char	err_file[MAX_LN] = { 0 };    /* Error log (redirected stderr).	*/
char	mat_db_file[MAX_LN] = { 0 }; /* Material database file.		*/
char	lgt_db_file[MAX_LN] = { 0 }; /* Light source database file.	*/
char	ir_db_file[MAX_LN] = { 0 };  /* IR database file.		*/
char	fb_file[MAX_LN] = { 0 };     /* Raster image output.		*/
char	ir_file[MAX_LN] = { 0 };     /* IR input data.			*/

/* Global buffers and pointers.						*/
char	input_ln[BUFSIZ];
char	prompt[MAX_LN];
char	version[] = "$Revision$";
char	title[TITLE_LEN];
char	timer[TIMER_LEN];
char	script_file[MAX_LN];
char	*ged_file = NULL;

char	*beginptr;		     /* sbrk() at start of program.	*/

/* Unit vectors representing horizontal and vertical directions of grid	*/
fastf_t	grid_hor[3], grid_ver[3];

/* Position of grid in model space.					*/
fastf_t	grid_loc[3];

/* Unit vector representing the incident ray in model space.		*/
fastf_t	modl_radius;		/* Radius of model (bounding sphere).	*/

/* Location of center of model (calculated from bounding RPP).		*/
fastf_t	modl_cntr[3];

/* Conversion degrees to radians.					*/
fastf_t	degtorad = 0.0174532925;

/* Translations of grid in plane of view.				*/
fastf_t	x_grid_offset = 0.0, y_grid_offset = 0.0;

/* Size of grid in normalized to model diameter.			*/
fastf_t	grid_scale = 1.0;

/* Distance of grid from the model centroid measured in milimeters.	*/
fastf_t	grid_dist = 0.0;

/* Rotation of grid around viewing axis (radians).			*/
fastf_t grid_roll = 0.0;

fastf_t	bg_coefs[3];		/* Background RGB coefficients.		*/
fastf_t	rel_perspective = 0.25;	/* Manual perspective adjustment.	*/
fastf_t	sample_sz;		/* Over-sampling ratio (aperture^2).	*/
fastf_t view_rots[16];		/* Store 4x4 MGED saved view matrix.	*/
fastf_t	view2model[16];		/* View-to-model matrix from view_rots.	*/
fastf_t	view_size;		/* Absolute grid size from MGED view.	*/
fastf_t	cell_sz = 0.0;		/* Cell size of grid in target coords.	*/

int	anti_aliasing = FALSE;	/* Anti-aliasing thru over-sampling.	*/
int	aperture_sz = 1;	/* Size of window for over-sampling.	*/
int	background[3];		/* Background as RGB values.		*/
int	co;			/* Number of columns on screen/layer.	*/
int	tracking_cursor=FALSE;	/* Cursor is on by default.		*/

int	grid_position = FALSE;	/* Did user set distance yet.		*/
int	save_view_flag = FALSE;	/* View specified in "model2view".	*/
int	hiddenln_draw = FALSE;	/* Generate hidden-line drawing.	*/
int	ir_aperture;		/* Size of window IR data over-sampling.*/
int	ir_noise = 2;		/* Threshold for subdivision of octree.	*/
int	ir_min = ABSOLUTE_ZERO;	/* IR input temperature ranges.		*/
int	ir_max = ABSOLUTE_ZERO;
int	ir_paint;		/* For temperature-to-location mapping.	*/
int	ir_doing_paint = FALSE;	/* Is user doing above mapping.		*/
int	ir_offset = FALSE;	/* Has user specified auto mapping.	*/
int	ir_mapx, ir_mapy;	/* Auto mapping offsets for above.	*/
int	ir_mapping = IR_OFF;	/* IR mapping.				*/
int	frame_no = 0;		/* Current frame being processed.	*/
int	user_interrupt = FALSE;	/* User-level interrupt of raytrace.	*/
int	fatal_error = FALSE;	/* Fatal error, must abort raytrace.	*/
int	lgt_db_size = 0;	/* Current size of light data base.	*/
int	max_bounce = 0;		/* Recursion level for raytracer.	*/
int	pix_buffered = B_LINE;	/* Scan line buffering is default.	*/
int	tty;		/* Is input attached to a terminal.		*/
int	grid_sz = 32;	/* Default resolution 32x32 pixels.		*/
int	grid_x_org = 0;	/* Grid x position to begin ray-tracing.	*/
int	grid_x_fin = 32;/* Grid x position to end ray-tracing.		*/
int	grid_x_cur = 0; /* Grid x current position.			*/
int	grid_y_cur = 0; /* Grid y current position.			*/
int	grid_y_org = 0;	/* Grid y position to begin ray-tracing.	*/
int	grid_y_fin = 32;/* Grid y position to end ray-tracing.		*/
int	query_region = FALSE;	/* If (true) spit out region info.	*/
int	report_overlaps = TRUE;	/* If (false) shut-up about overlaps.	*/
int	sgi_console = FALSE;	/* Logged in to IRIS console.		*/
int	sgi_usemouse = FALSE;	/* User wants to use the IRIS mouse.	*/
int	x_fb_origin = 0;/* Display origin left-most pixel to display.	*/
int	y_fb_origin = 0;/* Display origin top-most pixel to display.	*/
int	li;		/* Number of lines in window.			*/
#ifdef PARALLEL
int	npsw = MAX_PSW;	/* Number of worker PSWs to run.		*/
struct resource	resource[MAX_PSW]; /* Memory resources.			*/
#endif
struct rt_i	*rt_ip;	/* Globals from RT library.			*/
