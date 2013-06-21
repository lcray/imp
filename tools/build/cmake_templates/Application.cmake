# Autogenerated file, run tools/build/setup_cmake.py to regenerate

imp_get_process_exit_code("setup_application %(name)s" status ${PROJECT_BINARY_DIR}
                          COMMAND ${PROJECT_SOURCE_DIR}/tools/build/setup_application.py
                          --name=%(name)s
                          --datapath=${IMP_DATAPATH}
                           --source=${PROJECT_SOURCE_DIR})
if(${status} EQUAL 0)
include_directories(%(includepath)s)
link_directories(%(libpath)s)
add_definitions("-DIMP_EXECUTABLE")

%(bins)s

set(pybins %(pybins)s)
foreach (pybin ${pybins})
  install(PROGRAMS ${pybin} DESTINATION ${CMAKE_INSTALL_BINDIR})
endforeach(pybin)

set(pytests %(pytests)s)
foreach (test ${pytests})
  GET_FILENAME_COMPONENT(name ${test} NAME)
  add_test("%(name)s.${name}" ${IMP_TEST_SETUP} python ${test})
  set_tests_properties("%(name)s.${name}" PROPERTIES LABELS "IMP.%(name)s;test")
endforeach(test)

if(IMP_DOXYGEN_FOUND)
# documentation

file(GLOB docs ${PROJECT_SOURCE_DIR}/applications/%(name)s/*.dox
               ${PROJECT_SOURCE_DIR}/applications/%(name)s/*.md)

add_custom_command(OUTPUT ${PROJECT_BINARY_DIR}/doxygen/%(name)s/tags ${PROJECT_BINARY_DIR}/doc/html/%(name)s/index.html
   COMMAND mkdir -p doc/html
   COMMAND ${IMP_DOXYGEN_EXECUTABLE} ../../doxygen/%(name)s/Doxyfile
   COMMAND ${PROJECT_SOURCE_DIR}/tools/build/doxygen_patch_tags.py --module=%(name)s --file=../../doxygen/%(name)s/tags
   COMMAND ${PROJECT_SOURCE_DIR}/tools/build/doxygen_show_warnings.py --warn=../../doxygen/%(name)s/warnings.txt
   DEPENDS %(tags)s ${docs}
   WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/doxygen/%(name)s/
   COMMENT "Running doxygen on %(name)s")

add_custom_target("IMP.%(name)s-doc" ALL DEPENDS ${PROJECT_BINARY_DIR}/doxygen/%(name)s/tags)

set(IMP_%(NAME)s_DOC "IMP.%(name)s-doc" CACHE INTERNAL "" FORCE)

set(IMP_DOC_DEPENDS ${IMP_DOC_DEPENDS} ${IMP_%(NAME)s_DOC} CACHE INTERNAL "" FORCE)
endif(IMP_DOXYGEN_FOUND)

elseif(${status} EQUAL 1)
message("Application %(name)s disabled")
else()
message(FATAL_ERROR "setup_application failed ${status}")
endif()
