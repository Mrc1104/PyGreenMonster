# Name of this project
project(QwEPICS VERSION 2.0.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

#----------------------------------------------------------------------------
# Sources and headers
#
file(GLOB SRC CONFIGURE_DEPENDS
		 src/*.cc
)
file(GLOB INC CONFIGURE_DEPENDS
		 include/*.h
)
add_compile_options(-Wall -Wextra)

#----------------------------------------------------------------------------
# EPICS library
#
find_package(EPICS REQUIRED COMPONENTS
	Com ca 
)
if(NOT EPICS_FOUND)
	set(EPICS_FOUND FALSE PARENT_SCOPE)
	return()

endif()
set(EPICS_FOUND TRUE PARENT_SCOPE)

add_library(${PROJECT_NAME} SHARED ${SRC})

target_link_libraries(${PROJECT_NAME} PUBLIC
	EPICS::Com
	EPICS::ca
)

target_sources(${PROJECT_NAME}
	PUBLIC
	FILE_SET HEADERS
	BASE_DIRS include
	FILES ${INC}
)

install(TARGETS ${PROJECT_NAME}
    EXPORT ${CMAKE_PROJECT_NAME}
    FILE_SET HEADERS
)
