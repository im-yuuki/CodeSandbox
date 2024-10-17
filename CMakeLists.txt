cmake_minimum_required(VERSION 3.29)
project(CodeSandbox)

set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_executable(main
        src/main.cpp
        src/data/submissions.hpp
        src/data/enums.hpp
        src/data/problems.hpp
        src/utils/logging.hpp
        src/utils/env.hpp
        src/profiles/c17.hpp
        src/profiles/abstract.hpp
        src/utils/random.hpp
        src/utils/compare.hpp
        src/profiles/cpp17.hpp
        src/profiles/python312.hpp
)

find_package(Crow CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(main PRIVATE Crow::Crow asio::asio spdlog::spdlog)
