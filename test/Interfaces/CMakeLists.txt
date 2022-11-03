
option(NILAI_TEST_ALL_INTERFACES "Enable testing for all interfaces" OFF)
if (NILAI_TEST_ALL_INTERFACES)
    set(NILAI_TEST_TAS5707 ON CACHE BOOL "Enable testing for TAS5707" FORCE)
    set(NILAI_TEST_TAS5760 ON CACHE BOOL "Enable testing for TAS5760" FORCE)
endif ()

set(NILAI_INTERFACES_SOURCES)

option(NILAI_TEST_TAS5707 "Enable testing for TAS5707" OFF)
if (NILAI_TEST_TAS5707)
    add_subdirectory(TAS5707)
    set(NILAI_INTERFACES_SOURCES ${NILAI_INTERFACES_SOURCES} $<TARGET_PROPERTY:nilai_tas5707_test,SOURCES>)
endif ()

option(NILAI_TEST_TAS5760 "Enable testing for TAS5760" OFF)
if (NILAI_TEST_TAS5760)
    add_subdirectory(TAS5760)
    set(NILAI_INTERFACES_SOURCES ${NILAI_INTERFACES_SOURCES} $<TARGET_PROPERTY:nilai_tas5760_test,SOURCES>)
endif ()

set(NILAI_TEST_NAME nilai_interfaces_test)

if (DEFINED NILAI_SINGLE_TEST_EXE)
    add_custom_target(${NILAI_TEST_NAME}
            SOURCES ${NILAI_INTERFACES_SOURCES}
            )
endif ()
