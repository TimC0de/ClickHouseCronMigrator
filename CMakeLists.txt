cmake_minimum_required (VERSION 3.14)

project(ClickHouseCronMigrator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# uRuntime path setup
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)

# TODO: Add RELEASE mode compilation settings

# Find sources and headers
file(GLOB_RECURSE clickhouse_migration_sources
		"src/*.cpp" "src/**/*.cpp")

add_executable(${CMAKE_PROJECT_NAME} ${clickhouse_migration_sources})

# Add threads library
find_package(Threads REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} Threads::Threads)

# Add Boost library
find_package(Boost
	COMPONENTS "program_options"
	REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME} ${Boost_LIBRARIES})

# Building libs
add_subdirectory(libs)

# Install
install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY config DESTINATION ${CMAKE_INSTALL_PREFIX})