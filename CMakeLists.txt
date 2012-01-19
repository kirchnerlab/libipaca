PROJECT(LIBIPACA)

# require at least cmake 2.8
cmake_minimum_required(VERSION 2.8)

#############################################################################
# include cmake scripts
#############################################################################
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/macros)

# require out-of-source build
INCLUDE(require_out_of_src_build)
MACRO_REQUIRE_OUT_OF_SRC_BUILD("LIBIPACA requires out-of-source builds.")

# Git revision saved in variable LIBIPACA_VERSION
INCLUDE(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC LIBIPACA_VERSION)

#############################################################################
# Varaibles to upper string for easier comparisions
#############################################################################

STRING(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)

#############################################################################
# build libraries
#############################################################################
# require boost
#SET(Boost_USE_STATIC_LIBS OFF)
#SET(Boost_USE_MULTITHREAD OFF)
#make sure that at least version 1.46.0 is used
#SET(BOOST_MIN_VERSION "1.46.0")
#FIND_PACKAGE(Boost ${BOOST_MIN_VERSION} COMPONENTS thread REQUIRED)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

#############################################################################
# compiler Flags
#############################################################################

# Set default compile flags for GCC
if(CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_CXX_FLAGS_DEBUG  "-O0 -Wall -Wconversion -ggdb3")
    SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -ftree-vectorize -msse2")
    SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -O3")
endif(CMAKE_COMPILER_IS_GNUCXX)

#############################################################################
# Cmake generated header files
#############################################################################
CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/libipaca/config.hpp.cmake 
    ${LIBIPACA_BINARY_DIR}/include/libipaca/config.hpp
)

#############################################################################
# Cmake Options
#############################################################################

OPTION(ENABLE_TESTING "Compile tests" ON)
OPTION(ENABLE_COVERAGE "Enable GCov coverage analysis (defines a 'coverage' target and enforces static build of libipaca)" OFF)
OPTION(ENABLE_EXAMPLES "Compile examples" OFF)
OPTION(ENABLE_THREADING "Enable threading" FALSE)

#############################################################################
# global include dirs
#############################################################################

INCLUDE_DIRECTORIES(
    ${LIBIPACA_SOURCE_DIR}/include
    ${LIBIPACA_BINARY_DIR}/include
)

#############################################################################
# messages and error handling
#############################################################################
MESSAGE(STATUS "libipaca Git Commit: "${LIBIPACA_VERSION})
MESSAGE(STATUS "Build type: "${CMAKE_BUILD_TYPE})
MESSAGE(STATUS "CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}")
IF(ENABLE_THREADING)
    MESSAGE(STATUS "Threading enabled")
ELSE()
    MESSAGE(STATUS "Threading disabled")   
ENDIF()
IF(ENABLE_TESTING)
    MESSAGE(STATUS "Testing enabled")
ELSE()
    MESSAGE(STATUS "Testing disabled")
ENDIF()
IF(ENABLE_EXAMPLES)
    MESSAGE(STATUS "Examples enabled")
ELSE()
    MESSAGE(STATUS "Examples disabled")
ENDIF()
IF (ENABLE_COVERAGE)
    IF(CMAKE_BUILD_TYPE STREQUAL "DEBUG" AND ENABLE_TESTING)
        MESSAGE(STATUS "Coverage enabled")
    ELSEIF(NOT ENABLE_TESTING)
        MESSAGE(FATAL_ERROR "Coverage analysis requires Testing Enabled")
    ELSE()
        MESSAGE(FATAL_ERROR "Coverage analysis requires DEBUG build")
    ENDIF()
ELSE()
    MESSAGE(STATUS "Coverage disabled")
ENDIF()


#############################################################################
# code coverage analysis
#############################################################################

IF (ENABLE_COVERAGE)
    IF(CMAKE_BUILD_TYPE STREQUAL "DEBUG" AND ENABLE_TESTING)
        SET (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
        CONFIGURE_FILE(${LIBIPACA_SOURCE_DIR}/cmake/scripts/coverage.sh.in
            ${LIBIPACA_BINARY_DIR}/cmake/scripts/coverage.sh
            @ONLY IMMEDIATE
        )
        ADD_CUSTOM_TARGET(
            coverage COMMAND /bin/bash ${LIBIPACA_BINARY_DIR}/cmake/scripts/coverage.sh
        )        
    ENDIF()
ENDIF(ENABLE_COVERAGE)


#############################################################################
# tests
#############################################################################

IF (ENABLE_TESTING)
  INCLUDE(CTest)
  ENABLE_TESTING()
  ADD_SUBDIRECTORY(test)
ENDIF (ENABLE_TESTING)


#############################################################################
# build libipaca
#############################################################################

ADD_SUBDIRECTORY(src)

############################################################################
# examples
############################################################################
#
#IF (ENABLE_EXAMPLES)
#    ADD_SUBDIRECTORY(examples)
#ENDIF (ENABLE_EXAMPLES)

#############################################################################
# documentation
#############################################################################
# doxygen support
CONFIGURE_FILE(${LIBIPACA_SOURCE_DIR}/doc/Doxyfile.cmake
    ${LIBIPACA_BINARY_DIR}/doc/Doxyfile
    @ONLY IMMEDIATE
    )
CONFIGURE_FILE(${LIBIPACA_SOURCE_DIR}/doc/libipaca.png
    ${LIBIPACA_BINARY_DIR}/doc/libipaca.png
    COPYONLY
    )

FIND_PACKAGE(Doxygen)

# target for doxygen
ADD_CUSTOM_TARGET(doc COMMAND ${DOXYGEN} ${LIBIPACA_BINARY_DIR}/doc/Doxyfile)

#############################################################################
# installation
#############################################################################
# location, location, location
IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
SET(CMAKE_INSTALL_PREFIX
"/usr/local" CACHE PATH "libipaca install prefix" FORCE
)
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
# so that the dmg works also to the given location
SET(CPACK_PACKAGE_DEFAULT_LOCATION ${CMAKE_INSTALL_PREFIX})


# headers
INSTALL(DIRECTORY ${LIBIPACA_SOURCE_DIR}/include/libipaca
    DESTINATION include
    COMPONENT headers
    REGEX "/.git$" EXCLUDE
    PATTERN "config.hpp.cmake" EXCLUDE)

# config.h
INSTALL(FILES ${LIBIPACA_BINARY_DIR}/include/libipaca/config.hpp
    DESTINATION include/libipaca
    COMPONENT headers)

# documentation
INSTALL(DIRECTORY ${LIBIPACA_BINARY_DIR}/doc
    DESTINATION share/libipaca
    COMPONENT documentation
    PATTERN "Doxyfile" EXCLUDE)



##############################################################################
## exporting libipaca
##############################################################################
get_target_property(LIBIPACA_LIBRARY pipe LOCATION)

get_filename_component(LIBIPACA_INSTALL_LIBRARY ${LIBIPACA_LIBRARY} NAME)

set (LIBIPACA_LIBRARY ${CMAKE_INSTALL_PREFIX}/lib/${LIBIPACA_INSTALL_LIBRARY})
set (LIBIPACA_INCLUDE ${CMAKE_INSTALL_PREFIX}/include)

configure_file(
    "${PROJECT_SOURCE_DIR}/cmake/macros/libipacaConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/InstallFiles/libipacaConfig.cmake")
   
export(TARGETS pipe
  FILE "${PROJECT_BINARY_DIR}/InstallFiles/libipacaLibraryDepends.cmake")
 
export(PACKAGE libipaca)
    
if (UNIX)    
    INSTALL(FILES "${PROJECT_BINARY_DIR}/InstallFiles/libipacaConfig.cmake" DESTINATION /lib/libipaca/)
    INSTALL(FILES "${PROJECT_BINARY_DIR}/InstallFiles/libipacaLibraryDepends.cmake" DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/libipaca/cmake/)
elseif(WIN32)
    INSTALL(FILES "${PROJECT_BINARY_DIR}/InstallFiles/libipacaConfig.cmake" DESTINATION /cmake/)
    INSTALL(FILES "${PROJECT_BINARY_DIR}/InstallFiles/libipacaLibraryDepends.cmake" DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/libipaca/cmake/)
    
endif()

##############################################################################
## packaging
##############################################################################
## FIXME: make sure that the documentation is compiled and pdfs are 
##        generated prior to packaging!
#
set(CPACK_PACKAGE_NAME "libipaca")
set(CPACK_PACKAGE_VENDOR "kirchnerlab.org")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The isotope pattern calculator; version "$LIBIPACA_VERSION)

# CPack version strings
SET(CPACK_PACKAGE_VERSION_MAJOR 2)
SET(CPACK_PACKAGE_VERSION_MINOR 1)
SET(CPACK_PACKAGE_VERSION_PATCH 0)

SET(CPACK_RESOURCE_FILE_README "${LIBIPACA_SOURCE_DIR}/README.txt")
SET(CPACK_RESOURCE_FILE_LICENSE "${LIBIPACA_SOURCE_DIR}/LICENSE.txt")

set(CPACK_COMPONENTS_ALL libraries headers documentation examples)
set(CPACK_COMPONENT_LIBRARIES_DISPLAY_NAME "Libraries")
set(CPACK_COMPONENT_HEADERS_DISPLAY_NAME "C++ Development Headers")
set(CPACK_COMPONENT_DOCUMENTATION_DISPLAY_NAME "C++ Interface Documentation")
set(CPACK_COMPONENT_EXAMPLES_DISPLAY_NAME "Compiled Examples")

set(CPACK_COMPONENT_LIBRARIES_DESCRIPTION
    "Compiled libraries.")
set(CPACK_COMPONENT_HEADERS_DESCRIPTION
    "C++ header files for libipaca development.")
set(CPACK_COMPONENT_DOCUMENTATION_DESCRIPTION
    "The C++ libipaca interface documentation (Doxygen).")
set(CPACK_COMPONENT_EXAMPLES_DESCRIPTION
    "Compiled examples.")

INCLUDE(CPack)
