cmake_minimum_required(VERSION 3.15)
project(rugby_skeleton)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

set(CMAKE_C_STANDARD 11) # Requires C11 standard

set (CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME}
    main.cpp
    StateMachine/Behaviour.cpp
    StateMachine/Transition.cpp
    GameManager.cpp
    Entity.cpp
    Ball.cpp
    Player.cpp
    Utils.cpp)


target_link_libraries(${PROJECT_NAME}
	sfml-graphics 
	m 
	pthread 
	dl
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/StateMachine/>
)

target_compile_options(${PROJECT_NAME}
    PRIVATE "-g3"
	PRIVATE "-march=native"
)

# Checks if OSX and links appropriate frameworks (only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
