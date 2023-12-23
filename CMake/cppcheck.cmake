find_program(CPPCHECK cppcheck)

if(NOT CPPCHECK)
    message(STATUS "Cppcheck was not found, static-analysis targets are inaccessible")
else()
    cmake_host_system_information(RESULT N_PROC QUERY NUMBER_OF_LOGICAL_CORES)

    add_custom_target(cppcheck-check COMMAND ${CPPCHECK} 
        --check-level=exhaustive                                           # Cppcheck >=2.11
        --checkers-report=${CMAKE_BINARY_DIR}/cppcheck-checks-report.log   # Cppcheck >=2.12 - run with clean build dir, Cppcheck stores data there, so results can be different in subsequent runs
        --cppcheck-build-dir=${CMAKE_BINARY_DIR}
        --enable=all
        --error-exitcode=1
        --inconclusive                                                      # False positive can happen
        --language=c++
        --platform=native
        --suppress=missingIncludeSystem
        --suppress=*:*/QCustomPlot/*
        --library=qt                                                        # Solves 3rd party defines issues (here for Qt)
        --project=${CMAKE_BINARY_DIR}/compile_commands.json
        -i ${CMAKE_BINARY_DIR}                                              # Exclude build dir from analysis 
        -j ${N_PROC}
        COMMENT "Checking code with Cppcheck"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        USES_TERMINAL
    )
    
    message(STATUS "Cppcheck was found, 'cppcheck-check' target can be used")
endif()