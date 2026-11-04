function(add_binding_dep TARGET_NAME PY_BINDING_FILENAME LOC DEST)

    set(SRC_PY_FILE "${LOC}/${PY_BINDING_FILENAME}")
    set(DST_PY_FILE "${DEST}/${PY_BINDING_FILENAME}")

    add_custom_command(
        OUTPUT "${DST_PY_FILE}"
        DEPENDS "${SRC_PY_FILE}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SRC_PY_FILE}" "${DST_PY_FILE}"
        COMMENT "Syncing updated Python binding for ${TARGET_NAME}..."
    )

    add_custom_target(sync_${TARGET_NAME}_py ALL DEPENDS "${DST_PY_FILE}")
    add_dependencies(sync_${TARGET_NAME}_py ${TARGET_NAME})
endfunction()
