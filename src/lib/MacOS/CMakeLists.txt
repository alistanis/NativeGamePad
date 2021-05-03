if(APPLE)
    SET(CMAKE_C_COMPILER "/usr/bin/clang")
    SET(CMAKE_C_FLAGS "-mmacosx-version-min=11.3")
    add_library(${PROJECT_NAME}-MacOS
            ../../include/NGP_Types.h
            ../../include/NGP_Event.h
            ../../include/NGP_GamePad.h NGP_GamePad.m)

    find_library(APPKIT AppKit)
    find_library(GAME_CONTROLLER GameController)

    target_link_libraries(${PROJECT_NAME}-MacOS ${APPKIT} ${GAME_CONTROLLER})

    add_executable(${PROJECT_NAME}-MacOS-Demo ../../include/NGP_GamePad.h NGP_GamePad.m main.m)
    target_link_libraries(${PROJECT_NAME}-MacOS-Demo ${PROJECT_NAME}-MacOS)
endif()