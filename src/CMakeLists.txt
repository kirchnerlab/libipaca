
SET(SRCS 
    Mercury7Impl.cpp
    Stoichiometry.cpp
    Spectrum.cpp
)

ADD_LIBRARY(ipaca ${SRCS})

TARGET_LINK_LIBRARIES(ipaca
    #${Boost_LIBRARIES}
)
#
#
INSTALL(TARGETS ipaca
    EXPORT libipacaLibraryDepends
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    COMPONENT libraries
)
#

