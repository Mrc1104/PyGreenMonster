find_path(Remex_INCLUDE_DIR
    NAMES cMsg.h remexLib.h
    HINTS ENV CODA
    PATH_SUFFIXES common/include
)
if(NOT Remex_FIND_COMPONENTS)
    set(Remex_FIND_COMPONENTS cmsg remex)
endif()

foreach(comp ${Remex_FIND_COMPONENTS})
    # Search for the library on disk
    find_library(Remex_${comp}_LIBRARY
        NAMES ${comp}
        HINTS ENV CODA
        PATH_SUFFIXES Linux-x86_64/lib
    )
    # Check if this specific component was successfully found
    if(Remex_${comp}_LIBRARY)
        set(Remex_${comp}_FOUND TRUE)
    else()
        set(Remex_${comp}_FOUND FALSE)
    endif()
endforeach()

# Standardize status messaging (Found/Not Found) and error handling
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Remex
    REQUIRED_VARS Remex_remex_LIBRARY Remex_cmsg_LIBRARY Remex_INCLUDE_DIR 
	HANDLE_COMPONENTS
)

if(Remex_FOUND)
    foreach(comp ${Remex_FIND_COMPONENTS})
        if(Remex_${comp}_FOUND AND NOT TARGET Remex::${comp})
            add_library(Remex::${comp} UNKNOWN IMPORTED)
            set_target_properties(Remex::${comp} PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES "${Remex_INCLUDE_DIR}"
                IMPORTED_LOCATION "${Remex_${comp}_LIBRARY}"
            )
        endif()
    endforeach()
endif()
