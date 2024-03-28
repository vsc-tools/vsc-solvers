
include(GoogleTest)

find_package(Threads REQUIRED)

include (ExternalProject)

ExternalProject_Add(
  GTEST
  PREFIX gtest
  SOURCE_DIR ${PACKAGES_DIR}/gtest
  CMAKE_CACHE_ARGS
    -DCMAKE_C_COMPILER:STRING=${CMAKE_C_COMPILER}
    -DCMAKE_CXX_COMPILER:STRING=${CMAKE_CXX_COMPILER}
    -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
    -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
    -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
    -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}
    -DCMAKE_BUILD_TYPE:STRING=Release
    -DCMAKE_OSX_ARCHITECTURES:STRING=${CMAKE_OSX_ARCHITECTURES}
  )
  
ExternalProject_Get_Property(GTEST source_dir binary_dir)

# find_package(Gtest)

file(GLOB test_CPP_SRC
  "src/*.cpp"
  "${CMAKE_CURRENT_BINARY_DIR}/"
  )

add_custom_target(
    GEN_CODE_SNIPPETS
    COMMAND ${PYTHON} -m vsc_dataclasses.util.gen_cpp_dt_defs ${test_CPP_SRC} -o ${CMAKE_CURRENT_BINARY_DIR}/vscdefs -d ${CMAKE_CURRENT_BINARY_DIR}/vscdefs.d
)

add_library(libgtest IMPORTED STATIC GLOBAL)
add_dependencies(libgtest GTEST)

set_target_properties(libgtest PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/lib/libgtest.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
    )

add_library(libgtest_main IMPORTED STATIC GLOBAL)
add_dependencies(libgtest_main GTEST)

set_target_properties(libgtest_main PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/lib/libgtest_main.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
    )

add_library(libgmock IMPORTED STATIC GLOBAL)
add_dependencies(libgmock gtest)

set_target_properties(libgmock PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/lib/libgmock.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
    )
    
include_directories(
	"${source_dir}/googletest/include"
	"${source_dir}/googlemock/include"
    "${CMAKE_CURRENT_BINARY_DIR}/vscdefs"
	)

file(GLOB test_libvsc_solvers_SRC
  "src/*.h"
  "src/*.cpp"
  ${CMAKE_CURRENT_BINARY_DIR}/vscdefs.cpp
  )

add_executable(test_libvsc_solvers ${test_libvsc_solvers_SRC})
target_include_directories(test_libvsc_solvers PUBLIC
    "${source_dir}/googletest/include"
    "${vsc_dm_INCDIR}"
    "${debug_mgr_INCDIR}"
    )
   
target_link_directories(test_libvsc_solvers PRIVATE
    ${CMAKE_BINARY_DIR}/lib
    ${CMAKE_BINARY_DIR}/lib64
    ${CMAKE_BINARY_DIR}/gmp/lib
    "${vsc_dm_LIBDIR}"
    "${debug_mgr_LIBDIR}"
    )
  
target_link_libraries(test_libvsc_solvers 
    vsc-solvers 
	vsc-dm
    debug-mgr
	libgtest 
	libgtest_main 
	libgmock
	)
add_dependencies(test_libvsc_solvers GTEST GEN_CODE_SNIPPETS)
#set_property(TARGET test_libvsc PROPERTY INTERPROCEDURAL_OPTIMIZATION OFF)

gtest_discover_tests(test_libvsc_solvers)
