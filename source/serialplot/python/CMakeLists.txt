project(cpython)

find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

set(LIB_PY_DIRS "${Python3_RUNTIME_LIBRARY_DIRS}/Lib/site-packages")
set(LIBS_PY_DIRS "${Python3_RUNTIME_LIBRARY_DIRS}/Lib")

set(DIRECTORYS
        ${CMAKE_CURRENT_LIST_DIR}/include
        )

set(SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/src/pyhelper.cpp
        )

add_library(${PROJECT_NAME} STATIC ${SOURCES})
add_library(cpython::lib ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${DIRECTORYS}
        ${Python3_INCLUDE_DIRS}
        ${LIB_PY_DIRS}
        ${LIBS_PY_DIRS}
        )

target_sources(${PROJECT_NAME}
        PUBLIC
        ${DIRECTORYS}
        ${SOURCES}
        )

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        ${Python3_LIBRARIES}
        )

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${Python3_RUNTIME_LIBRARY_DIRS}/python${Python3_VERSION_MAJOR}${Python3_VERSION_MINOR}.dll ${CMAKE_BINARY_DIR}
        )

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_LIST_DIR}/python_modules ${CMAKE_BINARY_DIR}/source/serialplot/python_modules
        )
