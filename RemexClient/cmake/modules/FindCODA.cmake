find_path(CODA_INCLUDE_DIR
    NAMES cMsg.h remexLib.h
    HINTS ENV CODA
    PATH_SUFFIXES common/include
)
if(NOT CODA_FIND_COMPONENTS)
    set(CODA_FIND_COMPONENTS cmsg remex)
endif()

foreach(comp ${CODA_FIND_COMPONENTS})
    # Search for the library on disk
    find_library(CODA_${comp}_LIBRARY
        NAMES ${comp}
        HINTS ENV CODA
        PATH_SUFFIXES Linux-x86_64/lib
    )
    # Check if this specific component was successfully found
    if(CODA_${comp}_LIBRARY)
        set(CODA_${comp}_FOUND TRUE)
    else()
        set(CODA_${comp}_FOUND FALSE)
    endif()
endforeach()

# Standardize status messaging (Found/Not Found) and error handling
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CODA
    REQUIRED_VARS CODA_remex_LIBRARY CODA_cmsg_LIBRARY CODA_INCLUDE_DIR 
	HANDLE_COMPONENTS
)

if(CODA_FOUND)
    foreach(comp ${CODA_FIND_COMPONENTS})
        if(CODA_${comp}_FOUND AND NOT TARGET CODA::${comp})
            add_library(CODA::${comp} UNKNOWN IMPORTED)
            set_target_properties(CODA::${comp} PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES "${CODA_INCLUDE_DIR}"
                IMPORTED_LOCATION "${CODA_${comp}_LIBRARY}"
            )
        endif()
    endforeach()
endif()
