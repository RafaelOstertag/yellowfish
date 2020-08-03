set(sources 
            config.cxx
            imageretrievers/imageserver.cxx
            imageretrievers/localfile.cxx
            imageretrievers/nasapod.cxx
            imageretrievers/picsum.cxx
            imageretrievers/unsplash.cxx
            net/curl.cxx
            screens/clock.cxx
            screens/colors.cxx
            screens/image.cxx
            screens/sine.cxx
            screens/weather.cxx
            sdl/font.cxx
            sdl/memoryrwops.cxx
            sdl/renderer.cxx 
            sdl/sdl.cxx 
            sdl/window.cxx
            utils/dirlist.cxx
            utils/imageresizer.cxx
            utils/timekeeper.cxx
            weatherretriever/weather.cxx
            yellowfish.cxx
)

add_executable(yellowfish ${sources})
target_compile_options(yellowfish PRIVATE ${SDL2_CFLAGS_OTHER} ${CURL_CFLAGS_OTHER} ${MAGICKPP_CFLAGS} ${JSON_CFLAGS} -pthread)
target_include_directories(yellowfish PRIVATE ${CMAKE_CURRENT_BINARY_DIR} ${PROJECT_BINARY_DIR} PUBLIC ${SDL2_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS} ${MAGICKPP_INCLUDE_DIRS} ${JSON_INCLUDE_DIRS})
target_link_libraries(yellowfish ${SDL2_LDFLAGS} ${CURL_LDFLAGS} ${MAGICKPP_LDFLAGS} ${JSON_LDFLAGS})
target_link_options(yellowfish PRIVATE -pthread)
