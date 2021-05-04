if(APPLE)
    SET(CMAKE_C_COMPILER "/usr/bin/clang")
    SET(CMAKE_C_FLAGS "-mmacosx-version-min=11.3")
    add_library(${PROJECT_NAME}-MacOS ../NGP_GamePad.c CHUtil.m CHSinglyLinkedList.m NGP_GamePad.m)

    find_library(APPKIT AppKit)
    find_library(GAME_CONTROLLER GameController)
    find_library(IOKIT IOKit)

    target_link_libraries(${PROJECT_NAME}-MacOS ${APPKIT} ${GAME_CONTROLLER} ${IOKIT})

    add_executable(${PROJECT_NAME}-MacOS-Demo main.c)
    target_link_libraries(${PROJECT_NAME}-MacOS-Demo ${PROJECT_NAME}-MacOS)
endif()