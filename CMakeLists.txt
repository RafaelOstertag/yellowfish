cmake_minimum_required(VERSION 3.10)
enable_testing()

project(YellowFish VERSION 1.0.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -pedantic -Werror")
set(CMAKE_CXX_FLAGS_RELEASE "-g0 -O3 -Wall -pedantic -Werror -DNDEBUG")

include(FindPkgConfig)
pkg_check_modules(SDL2 REQUIRED sdl2 SDL2_ttf SDL2_image)
pkg_check_modules(CURL REQUIRED libcurl)
pkg_check_modules(JSON REQUIRED jsoncpp)
pkg_check_modules(CPPUNIT REQUIRED cppunit)
pkg_search_module(MAGICKPP REQUIRED Magick++ GraphicsMagick++)

add_subdirectory(src)
add_subdirectory(tests)

find_file(CLOCK_FONT DejaVuSerif-Bold.ttf /usr/share/fonts/dejavu /usr/share/fonts/truetype/dejavu /usr/share/fonts/dejavu-serif-fonts DOC "Clock font file")
set(LOCAL_IMAGE_DIR /var/lib/yellowfish/images CACHE FILEPATH "Compile time path to image directory")

install(TARGETS yellowfish DESTINATION bin)

configure_file(yellowfish.hh.in yellowfish.hh)

