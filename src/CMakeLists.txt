
SET(SRCS 
    Dummy.cpp
)

ADD_LIBRARY(ipaca ${SRCS})

TARGET_LINK_LIBRARIES(ipaca
    #${Boost_LIBRARIES}
)


INSTALL(TARGETS ipaca
    EXPORT libipacaLibraryDepends
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    COMPONENT libraries
)


