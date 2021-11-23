# Build ClickHouse C++ client
add_subdirectory(clickhouse-cpp)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC clickhouse-cpp clickhouse-cpp/contrib)
target_link_libraries(${CMAKE_PROJECT_NAME} clickhouse-cpp-lib)

# Install ClickHouse
install(TARGETS clickhouse-cpp-lib DESTINATION ${CMAKE_INSTALL_PREFIX})

# Build libcron
add_subdirectory(libcron/libcron)

target_include_directories(${CMAKE_PROJECT_NAME}
		PUBLIC libcron/libcron/externals/date/include libcron/libcron/include)

target_link_directories(${CMAKE_PROJECT_NAME} PUBLIC "libcron/libcron/out/${CMAKE_BUILD_TYPE}")
target_link_libraries(${CMAKE_PROJECT_NAME} libcron)

# Install libcron
install(TARGETS libcron DESTINATION ${CMAKE_INSTALL_PREFIX})

# Building loguru
add_library(loguru
	loguru/loguru.cpp)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC loguru)
target_link_libraries(${CMAKE_PROJECT_NAME} loguru ${CMAKE_DL_LIBS})

# Install loguru
install(TARGETS loguru DESTINATION ${CMAKE_INSTALL_PREFIX})