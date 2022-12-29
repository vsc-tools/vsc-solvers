
file(GLOB libvsc_solvers_SRC
  "*.h"
  "*.cpp"
  )
  
add_library(vsc-solvers SHARED ${libvsc_solvers_SRC})

target_include_directories(vsc-solvers PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/include
    ${CMAKE_BINARY_DIR}/include
    ${Boolector_INCLUDE_DIR}
    ${DEBUG_MGR_DIR}/src/include
    ${LIBVSC_DM_DIR}/src/include
    )
   
target_link_directories(vsc-solvers PRIVATE
    ${CMAKE_BINARY_DIR}/lib
    ${CMAKE_BINARY_DIR}/lib64
    ${CMAKE_BINARY_DIR}/gmp/lib
    )
  
target_link_libraries(vsc-solvers
    vsc-dm 
    debug-mgr
	bitwuzla
	boolector
	btor2parser
	cadical
	gmp
	)
add_dependencies(vsc-solvers Boolector Bitwuzla LibVSC-DM DebugMgr)

add_library(vsc-solvers_static STATIC ${libvsc_solvers_SRC})

target_include_directories(vsc-solvers_static PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/include
    ${CMAKE_BINARY_DIR}/include
    ${Boolector_INCLUDE_DIR}
    ${DEBUG_MGR_DIR}/src/include
    ${LIBVSC_DM_DIR}/src/include
    )
   
target_link_directories(vsc-solvers_static PRIVATE
    ${CMAKE_BINARY_DIR}/lib
    ${CMAKE_BINARY_DIR}/lib64
    )

add_dependencies(vsc-solvers_static Boolector Bitwuzla)

install(TARGETS vsc-solvers_static
    DESTINATION lib
    EXPORT libvsc-solvers-targets)

install(TARGETS vsc-solvers
    DESTINATION lib
    EXPORT libvsc-solvers-targets)

