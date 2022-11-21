include_guard(GLOBAL)

function(_set_cxx_standard)
    set_target_properties(${CURRENT_PROJECT_SANITIZED_NAME} PROPERTIES CXX_STANDARD 23)
endfunction()

function(_force_pdbs)
    if(MSVC AND CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_options(${CURRENT_PROJECT_SANITIZED_NAME} PRIVATE /Zi)
        set_target_properties(${CURRENT_PROJECT_SANITIZED_NAME} PROPERTIES
            LINK_FLAGS "/DEBUG /OPT:REF /OPT:ICF"
            COMPILE_PDB_NAME ${CURRENT_PROJECT_SANITIZED_NAME}
            COMPILE_PDB_OUTPUT_DIR ${CMAKE_BINARY_DIR}
        )
    endif()
endfunction()

function(_add_platform_macro)
    target_compile_definitions(${CURRENT_PROJECT_SANITIZED_NAME} PRIVATE CONFIG_VERSION_PLATFORM=${PROJECT_PLATFORM} CONFIG_VERSION_PLATFORM_${PROJECT_PLATFORM})
endfunction()

function(_disable_stdlib)
    target_compile_options(${CURRENT_PROJECT_SANITIZED_NAME} PRIVATE -nostdlib -fno-exceptions)
endfunction()

function(_use_pic)
    set_target_properties(${CURRENT_PROJECT_SANITIZED_NAME} PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
endfunction()

function(_report_undefined_refs)
    set_target_properties(${CURRENT_PROJECT_SANITIZED_NAME} PROPERTIES LINK_OPTIONS "--no-undefined")
endfunction()

function(_enable_clang_tidy)
    set_target_properties(${CURRENT_PROJECT_SANITIZED_NAME} PROPERTIES CXX_CLANG_TIDY "clang-tidy")
    set_target_properties(${CURRENT_PROJECT_SANITIZED_NAME} PROPERTIES C_CLANG_TIDY "clang-tidy")
endfunction()


function(_set_default_properties)
    _set_cxx_standard()
    _force_pdbs()
    _add_platform_macro()
    _enable_clang_tidy()
endfunction()
