add_executable(test-net-data test-net-data.cxx ${PROJECT_SOURCE_DIR}/src/net/curl.cxx)

add_test(NAME "net::data" COMMAND test-net-data)

target_compile_options(test-net-data PRIVATE ${SDL2_CFLAGS_OTHER} ${CURL_CFLAGS_OTHER} ${MAGICKPP_CFLAGS} ${JSON_CFLAGS} ${CPPUNIT_CFLAGS} -pthread)
target_include_directories(test-net-data PRIVATE ${PROJECT_SOURCE_DIR}/src ${CMAKE_CURRENT_BINARY_DIR} ${PROJECT_BINARY_DIR} PUBLIC ${SDL2_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS} ${MAGICKPP_INCLUDE_DIRS} ${JSON_INCLUDE_DIRS} ${CPPUNIT_INCLUDE_DIRS})
target_link_libraries(test-net-data ${SDL2_LDFLAGS} ${CURL_LDFLAGS} ${MAGICKPP_LDFLAGS} ${JSON_LDFLAGS} ${CPPUNIT_LDFLAGS})
target_link_options(test-net-data PRIVATE -pthread)
