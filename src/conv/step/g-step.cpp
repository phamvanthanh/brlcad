/*                    G -  S T E P . C P P
 * BRL-CAD
 *
 * Copyright (c) 2013 United States Government as represented by
 * the U.S. Army Research Laboratory.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 2.1 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this file; see the file named COPYING for more
 * information.
 */
/** @file step/g-step.cpp
 *
 * C++ main() for g-step converter.
 *
 */

#include "common.h"

#include <iostream>

#include "bu.h"

//
// step-g related headers
//
#include <BRLCADWrapper.h>
#include <STEPWrapper.h>

//
// include NIST step related headers
//
#include <sdai.h>
#include <STEPfile.h>
#ifdef AP203e2
#  include <SdaiAP203_CONFIGURATION_CONTROLLED_3D_DESIGN_OF_MECHANICAL_PARTS_AND_ASSEMBLIES_MIM_LF.h>
#else
#  include <SdaiCONFIG_CONTROL_DESIGN.h>
#endif


#include "Factory.h"
#include "schema.h"

void
usage()
{
    std::cerr << "Usage: g-step -o outfile.stp infile.g \n" << std::endl;
}


int
main(int argc, char *argv[])
{
    int ret = 0;

    // process command line arguments
    int c;
    char *output_file = (char *)NULL;
    while ((c = bu_getopt(argc, argv, "o:")) != -1) {
	switch (c) {
	    case 'o':
		output_file = bu_optarg;
		break;
	    default:
		usage();
		bu_exit(1, NULL);
		break;
	}
    }

    if (bu_optind >= argc || output_file == (char *)NULL) {
	usage();
	bu_exit(1, NULL);
    }

    argc -= bu_optind;
    argv += bu_optind;

    /* check our inputs/outputs */
    if (bu_file_exists(output_file, NULL)) {
	bu_exit(1, "ERROR: refusing to overwrite existing output file:\"%s\". Please remove file or change output file name and try again.", output_file);
    }

    if (!bu_file_exists(argv[0], NULL) && !BU_STR_EQUAL(argv[0], "-")) {
	bu_exit(2, "ERROR: unable to read input \"%s\" .g file", argv[0]);
    }

    std::string iflnm = argv[0];
    std::string oflnm = output_file;

    /* load .g file */
    BRLCADWrapper *dotg  = new BRLCADWrapper();
    if (!dotg) {
	std::cerr << "ERROR: unable to create BRL-CAD instance" << std::endl;
	return 3;
    }
    if (!dotg->load(iflnm)) {
	std::cerr << "ERROR: unable to open BRL-CAD input file [" << oflnm << "]" << std::endl;
        delete dotg;
        return 2;
    }

// TODO - Need to create proper load for .g files and OpenFile for .stp files - maybe rework slightly with an
// eye towards supporting read/write scenarios for both file types...
#if 0
    STEPWrapper *step = new STEPWrapper();

    /* load STEP file */
    if (step->load(iflnm)) {

	step->printLoadStatistics();

	BRLCADWrapper *dotg  = new BRLCADWrapper();
	if (!dotg) {
	    std::cerr << "ERROR: unable to create BRL-CAD instance" << std::endl;
	    ret = 3;
	} else {

	    std::cerr << "Writing output file [" << oflnm << "] ...";
	    if (dotg->OpenFile(oflnm.c_str())) {
		step->convert(dotg);
		std::cerr << "done!" << std::endl;
	    } else {
		std::cerr << "ERROR: unable to open BRL-CAD output file [" << oflnm << "]" << std::endl;
		ret = 4;
	    }

	    dotg->Close();
	    delete dotg;
	}
    }
    delete step;
    Factory::DeleteObjects();

#endif

    return ret;
}


// Local Variables:
// tab-width: 8
// mode: C++
// c-basic-offset: 4
// indent-tabs-mode: t
// c-file-style: "stroustrup"
// End:
// ex: shiftwidth=4 tabstop=8