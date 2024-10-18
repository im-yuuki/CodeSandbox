cmake_minimum_required(VERSION 3.29)
project(CodeSandbox)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_executable(main
        src/main.cpp

        src/data/submissions.hpp
        src/data/enums.hpp
        src/data/problems.hpp

        src/utils/logging.hpp
        src/utils/env.hpp
        src/utils/random.hpp
        src/utils/compare.hpp

        src/profiles/c.hpp
        src/profiles/abstract.hpp
        src/profiles/cpp.hpp
        src/profiles/python3.hpp
        src/utils/runguard.hpp
        src/profiles/register.hpp
)

# Install these packages with vcpkg
find_package(Crow CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

target_link_libraries(main PRIVATE Crow::Crow asio::asio spdlog::spdlog)

add_compile_options(
        -DNDEBUG
        -O3 -static
        -Wall -Wextra -Wpedantic
        -Wunused-variable
        -Wunused-parameter
        -Wunused-function
        -Wunused-but-set-variable
        -Wunused-private-field
        -Wunused-local-typedefs
        -Wunused-const-variable
        -Wunused-value
)
