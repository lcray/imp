# Autogenerated file, run tools/build/setup_cmake.py to regenerate

FILE(GLOB gensources
     "${CMAKE_BINARY_DIR}/src/%(name)s/*.cpp")

FILE(GLOB genheaders
     "${CMAKE_BINARY_DIR}/include/IMP/%(name)s/*.h")

include_directories(%(includepath)s)
link_directories(%(libpath)s)
add_definitions("-DIMP%(CPPNAME)s_EXPORTS")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${%(NAME)s_CXX_FLAGS}")

set(headers %(headers)s)

if(DEFINED IMP_%(NAME)s_LIBRARY_EXTRA_SOURCES)
  set_source_files_properties(${IMP_%(NAME)s_LIBRARY_EXTRA_SOURCES}
                              PROPERTIES GENERATED 1)
endif()
set_source_files_properties(${CMAKE_BINARY_DIR}/src/%(name)s_config.cpp
  PROPERTIES GENERATED 1)

if(IMP_HAS_CUDA)
  set(cuda_sources %(cuda_sources)s)
  foreach(src ${cuda_sources})
    cuda_compile(%(NAME)s_${src} ${src})
    list(APPEND cuda_targets ${%(NAME)s_${src}})
  endforeach()

  add_custom_target(IMP.%(name)s-cuda ${cuda_targets})
endif()

if(DEFINED IMP_%(name)s_IS_PER_CPP)
  set(sources %(sources)s)
  add_library(IMP.%(name)s-lib  ${IMP_LIB_TYPE} ${gensources} ${genheaders}
              ${headers} ${sources}
              ${CMAKE_BINARY_DIR}/src/%(name)s_config.cpp
              ${IMP_%(NAME)s_LIBRARY_EXTRA_SOURCES}
              ${cuda_targets}
              )
else()
  add_library(IMP.%(name)s-lib  ${IMP_LIB_TYPE} ${gensources} ${genheaders}
              ${headers} ${CMAKE_BINARY_DIR}/src/%(name)s_all.cpp
              ${CMAKE_BINARY_DIR}/src/%(name)s_config.cpp
              ${IMP_%(NAME)s_LIBRARY_EXTRA_SOURCES}
              ${cuda_targets}
              )
endif()
set_target_properties(IMP.%(name)s-lib PROPERTIES
                      OUTPUT_NAME imp_%(name)s)
set_property(TARGET "IMP.%(name)s-lib" PROPERTY FOLDER "IMP.%(name)s")

INSTALL(TARGETS IMP.%(name)s-lib DESTINATION ${CMAKE_INSTALL_LIBDIR})

add_dependencies(IMP.%(name)s-lib IMP.%(name)s-version)

if(DEFINED IMP_%(NAME)s_LIBRARY_EXTRA_DEPENDENCIES)
  add_dependencies(IMP.%(name)s-lib ${IMP_%(NAME)s_LIBRARY_EXTRA_DEPENDENCIES} ${cuda_targets})
endif()

target_link_libraries(IMP.%(name)s-lib ${imp_%(name)s_libs})

set(IMP_%(name)s_LIBRARY IMP.%(name)s-lib CACHE INTERNAL "" FORCE)
