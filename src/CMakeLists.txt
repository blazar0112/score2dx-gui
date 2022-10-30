find_package(Qt5 COMPONENTS Widgets Qml Quick Charts REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(fmt CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(ies CONFIG REQUIRED)
find_package(score2dx CONFIG REQUIRED)

set(MINGW_LIBRARY_NAMES
    gcc_s_seh-1 stdc++-6 winpthread-1
    curl-4 brotlicommon brotlidec crypto-1_1-x64 idn2-0 nghttp2-14 psl-5 ssh2-1 ssl-1_1-x64 zstd iconv-2
    intl-8 unistring-2
)
set(MINGW_LIBRARIES)
foreach(LIB ${MINGW_LIBRARY_NAMES})
    set(MINGW_LIB_PATH $ENV{MINGW64_ROOT}/bin/lib${LIB}.dll)
    if (EXISTS ${MINGW_LIB_PATH} AND NOT IS_DIRECTORY ${MINGW_LIB_PATH})
        file(TO_CMAKE_PATH ${MINGW_LIB_PATH} FOUND_MINGW_LIB)
        list(APPEND MINGW_LIBRARIES ${FOUND_MINGW_LIB})
        message(STATUS "Found MINGW Lib: ${FOUND_MINGW_LIB}")
    endif()
endforeach(LIB)

list(APPEND MINGW_LIBRARIES $ENV{MINGW64_ROOT}/bin/zlib1.dll)

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

set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/qml/ScoreViewer.rc")
add_executable(ScoreViewer WIN32
    ${PUBLIC_HEADERS} ${HEADERS} ${SOURCES}
    app/ScoreViewer.cpp
    qml.qrc
    ${APP_ICON_RESOURCE_WINDOWS}
)
target_compile_options(ScoreViewer PRIVATE -std=c++20 -pedantic -Wall -Wextra -Werror -g -DFMT_HEADER_ONLY)
target_include_directories(ScoreViewer SYSTEM PUBLIC
    ${FMT_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
)
target_include_directories(ScoreViewer PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}>
)
target_link_libraries(ScoreViewer
    stdc++fs
    ies::ies
    curl
    score2dx::score2dx
    Qt5::Widgets
    Qt5::Qml
    Qt5::Quick
    Qt5::Charts
)

add_custom_command(TARGET ScoreViewer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${MINGW_LIBRARIES} $<TARGET_FILE:ies::ies> $<TARGET_FILE:score2dx::score2dx> ${GTEST_LIBRARY}
        $ENV{MINGW64_ROOT}/ssl/certs/ca-bundle.crt
        $<TARGET_FILE_DIR:ScoreViewer>
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        $<TARGET_FILE_DIR:score2dx::score2dx>/../table
        $<TARGET_FILE_DIR:ScoreViewer>/table
)
