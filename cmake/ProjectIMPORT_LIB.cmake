function(_define_project_internal)
    message(FATAL_ERROR "The import only library ${CURRENT_PROJECT_NAME} cannot have any sources")
endfunction()

function(_define_project_alias_internal)

endfunction()

add_library(${CURRENT_PROJECT_NAME} STATIC IMPORTED GLOBAL)
set_target_properties(${CURRENT_PROJECT_NAME} PROPERTIES IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/${CURRENT_PROJECT_IMPORTED_LOCATION}")
target_include_directories(${CURRENT_PROJECT_NAME} INTERFACE .)