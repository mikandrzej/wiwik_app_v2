find_program(CLANG_TIDY clang-tidy)

if(NOT CLANG_TIDY)
    message(STATUS "Clang-Tidy not found!")
else()
    message(STATUS "Clang-Tidy was found, 'clang-tidy' and 'clang-tidy-fix' targets can be used")

    file(WRITE ${CMAKE_BINARY_DIR}/.clang-tidy "---\nChecks: '-*,bugprone-no-escape'\n...\n")

    add_custom_target(clang-tidy-check COMMAND clang-tidy ${ALL_SOURCES} -p=${CMAKE_BINARY_DIR} -use-color
        COMMENT "Checking code with Clang-Tidy"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        USES_TERMINAL
    )

    add_custom_target(clang-tidy-fix COMMAND clang-tidy ${ALL_SOURCES} -p=${CMAKE_BINARY_DIR} -use-color -fix-errors --format-style=file
        COMMENT "Applying automatic fixes to code with clang-tidy"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        USES_TERMINAL
    )
endif()

find_program(RUN_CLANG_TIDY run-clang-tidy)
if(NOT RUN_CLANG_TIDY)
    message(STATUS "Run-Clang-Tidy not found!")
else()
    message(STATUS "Run-Clang-Tidy was found, 'run-clang-tidy' and 'run-clang-tidy-fix' targets can be used")

    add_custom_target(run-clang-tidy-check COMMAND ${RUN_CLANG_TIDY} -p=${CMAKE_BINARY_DIR} -use-color
        COMMENT "Checking code with run-clang-tidy (parallel)"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        USES_TERMINAL
    )

    add_custom_target(run-clang-tidy-fix COMMAND ${RUN_CLANG_TIDY} -p=${CMAKE_BINARY_DIR} -use-color -format -fix
        COMMENT "Checking code with run-clang-tidy (parallel)"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        USES_TERMINAL
    )
endif()