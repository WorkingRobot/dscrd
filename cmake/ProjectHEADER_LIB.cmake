function(_define_project_internal)
    if(CURRENT_PROJECT_SOURCES)
        message(FATAL_ERROR "The header only library ${CURRENT_PROJECT_NAME} cannot have explicitly defined sources")
    endif()

    add_library(${CURRENT_PROJECT_SANITIZED_NAME} INTERFACE)
    target_include_directories(${CURRENT_PROJECT_SANITIZED_NAME} INTERFACE .)
endfunction()

function(_define_project_alias_internal)
    add_library(${CURRENT_PROJECT_NAME} ALIAS ${CURRENT_PROJECT_SANITIZED_NAME})
endfunction()

end_sources()