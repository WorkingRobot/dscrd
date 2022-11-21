include_guard(GLOBAL)

function(_sanitize_project_name VARIABLE NAME)
    string(REPLACE "::" "-" NAME ${NAME})
    set(${VARIABLE} ${NAME} PARENT_SCOPE)
endfunction()

function(_project_internal)
    _sanitize_project_name(CURRENT_PROJECT_SANITIZED_NAME ${CURRENT_PROJECT_NAME})
    hoist_variable(CURRENT_PROJECT_SANITIZED_NAME)

    include(Project${CURRENT_PROJECT_TYPE})
endfunction()

macro(create_project NAME TYPE)
    set(CURRENT_PROJECT_NAME ${NAME})
    set(CURRENT_PROJECT_TYPE ${TYPE})

    cmake_parse_arguments(CURRENT_PROJECT "IS_SIMPLE;IS_DISABLED" "IMPORTED_LOCATION" "PLATFORMS" ${ARGN})

    if (CURRENT_PROJECT_IS_DISABLED)
        message("Not building ${CURRENT_PROJECT_NAME} because it's disabled")
        return()
    endif()

    if (CURRENT_PROJECT_PLATFORMS)
        if (NOT PROJECT_PLATFORM IN_LIST CURRENT_PROJECT_PLATFORMS)
            message("Not building ${CURRENT_PROJECT_NAME} because it's unsupported (only for ${CURRENT_PROJECT_PLATFORMS})")
            return()
        endif()
    endif()

    _project_internal()
endmacro()

function(_define_project_alias)
    if (CURRENT_PROJECT_NAME STREQUAL CURRENT_PROJECT_SANITIZED_NAME)
        return()
    endif()

    _define_project_alias_internal()
endfunction()

function(_define_project)
    _define_project_internal()

    _define_project_alias()
endfunction()

set_property(GLOBAL PROPERTY ALL_SOURCES)
function(_push_sources)
    file(RELATIVE_PATH REL_PATH ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
    foreach(SOURCE ${CURRENT_PROJECT_SOURCES})
        list(APPEND PROJECT_SOURCES ${REL_PATH}/${SOURCE})
    endforeach()
    set_property(GLOBAL APPEND PROPERTY ALL_SOURCES ${PROJECT_SOURCES})
endfunction()

function(end_sources)
    _define_project()
    _push_sources()
endfunction()

function(self_link_libraries)
    target_link_libraries(${CURRENT_PROJECT_SANITIZED_NAME} ${ARGV})
endfunction()

function(self_compile_definitions)
    target_compile_definitions(${CURRENT_PROJECT_SANITIZED_NAME} ${ARGV})
endfunction()