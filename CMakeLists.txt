cmake_minimum_required(VERSION 3.14)

project(StockTrendChangePredictor VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)

# Check if we are in the top level project or not
if (NOT CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
message(STATUS "This project has a top-level one called [${CMAKE_PROJECT_NAME}]")
else()
message(STATUS "This project is a top-level one")
endif()

find_package(cpprestsdk REQUIRED)
find_package(spdlog REQUIRED)

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

#           CHECK ALL THE SUBMODULES
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/libs/CPPFinancialData/CMakeLists.txt")
    message(FATAL_ERROR "The CPPFinancialData submodules was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

add_subdirectory(libs)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC 
    cpprestsdk::cpprest
    spdlog
    FinancialData
    QuantLib
)

  