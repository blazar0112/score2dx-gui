find_package(Qt5 COMPONENTS Widgets Qml Quick Charts REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(ENV{icl_s2_DIR} $ENV{INSTALL_ROOT}/icl_s2/2.0.0/mingw64-10.2.0)
find_package(icl_s2)

set(ENV{score2dx_DIR} $ENV{INSTALL_ROOT}/score2dx)
find_package(score2dx REQUIRED)

# Header only libraries
set(FMT_INCLUDE_DIRS $ENV{FMT_ROOT}/include)
set(JSON_INCLUDE_DIRS $ENV{JSON_ROOT}/include)

set(MINGW_LIBRARY_NAMES gcc_s_seh-1 stdc++-6 winpthread-1)
set(MINGW_LIBRARIES)
foreach(LIB ${MINGW_LIBRARY_NAMES})
    set(MINGW_LIB_PATH $ENV{MINGW64_ROOT}/bin/lib${LIB}.dll)
    if (EXISTS ${MINGW_LIB_PATH} AND NOT IS_DIRECTORY ${MINGW_LIB_PATH})
        file(TO_CMAKE_PATH ${MINGW_LIB_PATH} FOUND_MINGW_LIB)
        list(APPEND MINGW_LIBRARIES ${FOUND_MINGW_LIB})
        message("Found MINGW Lib: ${FOUND_MINGW_LIB}")
    endif()
endforeach(LIB)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set_property(GLOBAL PROPERTY PROP_HEADERS)
set_property(GLOBAL PROPERTY PROP_SOURCES)
set_property(GLOBAL PROPERTY PROP_PUBLIC_HEADERS)
set_property(GLOBAL PROPERTY PROP_TEST_SOURCES)

add_subdirectory(gui)

get_property(HEADERS GLOBAL PROPERTY PROP_HEADERS)
get_property(SOURCES GLOBAL PROPERTY PROP_SOURCES)
get_property(PUBLIC_HEADERS GLOBAL PROPERTY PROP_PUBLIC_HEADERS)
get_property(TEST_SOURCES GLOBAL PROPERTY PROP_TEST_SOURCES)

add_library(gui SHARED ${PUBLIC_HEADERS} ${HEADERS} ${SOURCES})
target_compile_options(gui PRIVATE -std=c++20 -pedantic -Wall -Wextra -Werror -g -fPIC -DFMT_HEADER_ONLY)
target_include_directories(gui SYSTEM PUBLIC
    ${FMT_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
)
target_include_directories(gui PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}>
)
target_link_libraries(gui
    stdc++fs
    icl_s2::icl_s2
    score2dx::score2dx
    Qt5::Widgets
    Qt5::Qml
    Qt5::Quick
    Qt5::Charts
)

add_custom_command(TARGET gui POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${MINGW_LIBRARIES} $<TARGET_FILE:icl_s2::icl_s2> $<TARGET_FILE:score2dx::score2dx> ${GTEST_LIBRARY}
        $<TARGET_FILE_DIR:gui>
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        $<TARGET_FILE_DIR:score2dx::score2dx>/../table
        $<TARGET_FILE_DIR:gui>/table
)

add_executable(ScoreViewer WIN32 app/ScoreViewer.cpp qml.qrc)
#add_executable(ScoreViewer app/ScoreViewer.cpp qml.qrc)
target_compile_options(ScoreViewer PRIVATE -std=c++20 -pedantic -Wall -Wextra -Werror -g -fPIC -DFMT_HEADER_ONLY)
target_link_libraries(ScoreViewer
    gui
    Qt5::Widgets
    Qt5::Qml
    Qt5::Quick
    Qt5::Charts
)
