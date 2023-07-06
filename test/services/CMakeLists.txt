set(NILAI_TEST_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/serializer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/deserializer.cpp
        )

set(NILAI_TEST_NAME nilai_services_test)
message(STATUS "Building ${NILAI_TEST_NAME}")

if (DEFINED NILAI_SINGLE_TEST_EXE)
    add_custom_target(${NILAI_TEST_NAME}
            SOURCES ${NILAI_TEST_SOURCES})
else ()
    add_executable(${NILAI_TEST_NAME}
            ${NILAI_TEST_SOURCES}
            )

    if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
        set_target_properties(${NILAI_TEST_NAME}
                PROPERTIES SUFFIX .exe)
        gtest_discover_tests(${NILAI_TEST_NAME})
    else ()
        gtest_discover_tests(${NILAI_TEST_NAME})
    endif ()

    target_link_libraries(
            ${NILAI_TEST_NAME}
            gtest_main
    )
endif ()
