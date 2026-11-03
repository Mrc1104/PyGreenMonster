cmake_minimum_required(VERSION 3.31)
project(RemexClient VERSION 1.0.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

#----------------------------------------------------------------------------
# Sources and headers
#
file(GLOB SRC CONFIGURE_DEPENDS
		 src/*.cxx
)
file(GLOB INC CONFIGURE_DEPENDS
		 include/*.h
)

add_compile_options(-Wall -Wextra)

#----------------------------------------------------------------------------
# Remex library
#
find_package(Remex REQUIRED COMPONENTS
	cmsg remex
)

add_library(${PROJECT_NAME} SHARED ${SRC})
target_link_libraries(${PROJECT_NAME} PUBLIC
	Remex::cmsg
	Remex::remex
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

# Need to tell RemexClient.py where the lib resides
file(GENERATE
     OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/config/config.py"
	 INPUT "${CMAKE_CURRENT_SOURCE_DIR}/config/config.py.in"
)
