cmake_minimum_required(VERSION 3.31)
project(BMW VERSION 1.0.0 LANGUAGES CXX)

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
# EPICS library
#
find_package(EPICS COMPONENTS
	Com ca 
)


#----------------------------------------------------------------------------
# Remex library
#
add_library(${PROJECT_NAME} SHARED ${SRC})
target_link_libraries(${PROJECT_NAME} PUBLIC
	RemexClient
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

#----------------------------------------------------------------------------
# Remex library Python Bindings
#
#add_binding_dep(${PROJECT_NAME} RemexClient.py ${CMAKE_CURRENT_SOURCE_DIR} ${PYTHON_LIBRARY_DIR})

#----------------------------------------------------------------------------
# executables
#
file(GLOB exefiles CONFIGURE_DEPENDS
  main/*.cxx
)
foreach(file ${exefiles})
  get_filename_component(filename ${file} NAME_WE)
  string(TOLOWER ${filename} filelower)

  add_executable(${filelower} ${file})

  target_link_libraries(${filelower}
    PRIVATE
	  ${PROJECT_NAME}
  )
  target_compile_options(${filelower}
    PUBLIC
      ${${PROJECT_NAME_UC}_CXX_FLAGS_LIST}
    PRIVATE
      ${${PROJECT_NAME_UC}_DIAG_FLAGS_LIST}
  )
  if(${CMAKE_SYSTEM_NAME} MATCHES Linux)
    target_compile_options(${filelower} PUBLIC -fPIC)
  endif()

  install(TARGETS ${filelower}
    DESTINATION ${CMAKE_INSTALL_BINDIR}
  )
endforeach()

