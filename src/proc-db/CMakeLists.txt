set(PROC_DB_INCLUDE_DIRS
  ${WDB_INCLUDE_DIRS}
  ${OPENNURBS_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  )
LIST(REMOVE_DUPLICATES PROC_DB_INCLUDE_DIRS)
include_directories(${PROC_DB_INCLUDE_DIRS})


# DEVELOPMENT TESTING TOOLS
#
# these are experimental that are not intended for general use.  new
# applications should start as NO_INSTALL until they reach a state
# where they provide some conceivable value to an end user.
BRLCAD_ADDEXEC(bottest bottest.c libwdb NO_INSTALL)
BRLCAD_ADDEXEC(brep_cobb brep_cobb.cpp "libwdb;${OPENNURBS_LIBRARY}" NO_STRICT NO_INSTALL)
BRLCAD_ADDEXEC(brep_cube "brep_cube.cpp;twistedcube.cpp" "libwdb;${OPENNURBS_LIBRARY}" NO_STRICT NO_INSTALL)
BRLCAD_ADDEXEC(brep_simple "brep_simple.cpp;twistedcube.cpp" "libwdb;${OPENNURBS_LIBRARY}" NO_STRICT NO_INSTALL)
BRLCAD_ADDEXEC(brepintersect brepintersect.cpp "libwdb;${OPENNURBS_LIBRARY}" NO_STRICT NO_INSTALL)
BRLCAD_ADDEXEC(breplicator breplicator.cpp "libwdb;${OPENNURBS_LIBRARY}" NO_STRICT NO_INSTALL)
BRLCAD_ADDEXEC(contours contours.c libwdb NO_INSTALL)
BRLCAD_ADDEXEC(csgbrep csgbrep.cpp libwdb NO_STRICT NO_INSTALL)
BRLCAD_ADDEXEC(nmgmodel nmgmodel.c libwdb NO_INSTALL)
BRLCAD_ADDEXEC(pipetest pipetest.c libwdb NO_INSTALL)
BRLCAD_ADDEXEC(spltest spltest.c libwdb NO_INSTALL)
BRLCAD_ADDEXEC(surfaceintersect "surfaceintersect.cpp;twistedcube.cpp" "libwdb;${OPENNURBS_LIBRARY}" NO_STRICT NO_INSTALL)
BRLCAD_ADDEXEC(tea_nmg tea_nmg.c libwdb NO_INSTALL)
BRLCAD_ADDEXEC(tube tube.c "libwdb;${M_LIBRARY}" NO_INSTALL)
BRLCAD_ADDEXEC(wdb_example wdb_example.c libwdb NO_INSTALL)


# MORE THAN DEVELOPMENT TESTING
#
# these have reached a level of maturity where they provide some
# (however remote) value to an end user, aren't just debugging aids,
# but haven't yet reached a level of maturity where they might belong
# elsewhere in the package (like src/shapes).
BRLCAD_ADDEXEC(brickwall brickwall.c libwdb)
BRLCAD_ADDEXEC(cchannel cchannel.c "libwdb;${M_LIBRARY}")
BRLCAD_ADDEXEC(clutter "clutter.c;common.c" "libwdb;${M_LIBRARY}")
BRLCAD_ADDEXEC(globe globe.c libwdb)
BRLCAD_ADDEXEC(kurt kurt.c "libwdb;${M_LIBRARY}")
BRLCAD_ADDEXEC(lens lens.c "libwdb;${M_LIBRARY}")
BRLCAD_ADDEXEC(masonry masonry.c "libwdb;${M_LIBRARY}")
BRLCAD_ADDEXEC(menger menger.c "libwdb;${M_LIBRARY}")
BRLCAD_ADDEXEC(metaball metaball.c "libwdb;${M_LIBRARY}")
BRLCAD_ADDEXEC(mkbuilding "mkbuilding.c;makebuilding/makebuilding.c" libwdb)
BRLCAD_ADDEXEC(molecule molecule.c libwdb)
BRLCAD_ADDEXEC(pdb-g pdb-g.c libwdb)
BRLCAD_ADDEXEC(pipe pipe.c "libwdb;${M_LIBRARY}")
BRLCAD_ADDEXEC(pix2g pix2g.c libwdb)
BRLCAD_ADDEXEC(pyramid pyramid.c "libwdb;${M_LIBRARY}")
BRLCAD_ADDEXEC(rawbot rawbot.c libwdb)
BRLCAD_ADDEXEC(ringworld ringworld.c libwdb)
BRLCAD_ADDEXEC(room "room.c;common.c" "libwdb;${M_LIBRARY}")
BRLCAD_ADDEXEC(sketch sketch.c libwdb)
BRLCAD_ADDEXEC(sphflake sphflake.c "libwdb;${M_LIBRARY}")
BRLCAD_ADDEXEC(tea tea.c libwdb)
BRLCAD_ADDEXEC(torii torii.c libwdb)
BRLCAD_ADDEXEC(vegetation vegetation.c "libwdb;${M_LIBRARY}")
BRLCAD_ADDEXEC(wavy wavy.c libwdb)


set(proc-db_ignore_files
  twistedcube.h
  brepintersect.h
  metaballs.pl
  mkbuilding.h
  sgi.sh
  spiral.pl
  surfaceintersect.h
  tea.h
  vegetation.h
  )
CMAKEFILES(${proc-db_ignore_files})

# Local Variables:
# tab-width: 8
# mode: cmake
# indent-tabs-mode: t
# End:
# ex: shiftwidth=2 tabstop=8
