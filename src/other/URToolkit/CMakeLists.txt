IF(COMMAND CMAKE_POLICY)
  if ("${CMAKE_VERSION}" VERSION_GREATER 2.99)
	  CMAKE_POLICY(SET CMP0026 OLD)
  endif ("${CMAKE_VERSION}" VERSION_GREATER 2.99)
ENDIF(COMMAND CMAKE_POLICY)

IF(NOT MSVC)
  MACRO(URT_EXEC execname srcs)
    STRING(REGEX REPLACE " " ";" srcslist "${srcs}")
    add_executable(${execname} ${srcslist})
    target_link_libraries(${execname} ${UTAHRLE_LIBRARY})
    INSTALL(TARGETS ${execname} RUNTIME DESTINATION bin)
  ENDMACRO()

  ADD_SUBDIRECTORY(cnv)
  ADD_SUBDIRECTORY(man)
  ADD_SUBDIRECTORY(tools)
ENDIF(NOT MSVC)
