find_program(CLANG_FORMAT clang-format)

if(NOT CLANG_FORMAT)
    message(STATUS "Clang-Format not found!")
else()
    message(STATUS "Clang-Format was found, 'clang-format' and 'clang-format-check' targets can be used")

    add_custom_target(clang-format COMMAND clang-format --style=file -i ${ALL_SOURCES} ${ALL_HEADERS}
        COMMENT "Formatting code with Clang-Format"
        USES_TERMINAL
    )

    add_custom_target(clang-format-check COMMAND clang-format --style=file --dry-run --Werror ${ALL_SOURCES} ${ALL_HEADERS}
        COMMENT "Checking code style with Clang-Format"
        USES_TERMINAL
    )
endif()
