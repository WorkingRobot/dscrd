include_guard(GLOBAL)

function(add_dependency_pkgconfig NAME)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(PKGCONFIG_LIB REQUIRED ${NAME})
    target_link_libraries(${CURRENT_PROJECT_SANITIZED_NAME} ${VISIBILITY} ${PKGCONFIG_LIB_LIBRARIES})
    target_include_directories(${CURRENT_PROJECT_SANITIZED_NAME} ${VISIBILITY} ${PKGCONFIG_LIB_INCLUDE_DIRS})
endfunction()

function(add_dependency_manual INCLUDE_PATH LIBRARY_NAME)
    find_path(MANUAL_INCLUDE_DIR ${INCLUDE_PATH} REQUIRED)
    find_library(MANUAL_LIBRARY ${LIBRARY_NAME} REQUIRED)
    target_include_directories(${CURRENT_PROJECT_SANITIZED_NAME} ${VISIBILITY} ${MANUAL_INCLUDE_DIR})
    target_link_libraries(${CURRENT_PROJECT_SANITIZED_NAME} ${VISIBILITY} ${MANUAL_LIBRARY})
endfunction()

function(add_dependency_findpkg PKG_NAME)
    cmake_parse_arguments(PARSE_ARGV 1 FINDPKG "CONFIG" "" "LIBS")
    if (${FINDPKG_CONFIG})
        find_package(${PKG_NAME} CONFIG REQUIRED)
    else()
        find_package(${PKG_NAME} REQUIRED)
    endif()
    target_link_libraries(${CURRENT_PROJECT_SANITIZED_NAME} ${VISIBILITY} ${FINDPKG_LIBS})
endfunction()

function(add_dependency NAME VISIBILITY)
    cmake_language(CALL _dependency_${NAME})
endfunction()

function(_dependency_JSON)
    add_dependency_findpkg(RapidJSON CONFIG LIBS rapidjson)
endfunction()

function(_dependency_HTTP)
    add_dependency_findpkg(cpr CONFIG LIBS cpr::cpr)
endfunction()

function(_dependency_libbacktrace)
    add_dependency_manual(backtrace.h backtrace)
endfunction()

function(_dependency_breakpad)
    add_dependency_findpkg(unofficial-breakpad CONFIG LIBS unofficial::breakpad::libbreakpad unofficial::breakpad::libbreakpad_client)
endfunction()

function(_dependency_libfmt)
    add_dependency_findpkg(fmt CONFIG LIBS fmt::fmt)
endfunction()

function(_dependency_date)
    add_dependency_findpkg(date CONFIG LIBS date::date date::date-tz)
endfunction()

function(_dependency_OpenSSL)
    add_dependency_findpkg(OpenSSL LIBS OpenSSL::Crypto)
endfunction()