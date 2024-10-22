cmake_minimum_required(VERSION 3.29)
project(CodeSandbox)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -O3 -DNDEBUG")

add_executable(main
        src/main.cpp

        src/data/submissions.hpp
        src/data/enums.hpp
        src/data/problems.hpp

        src/utils/logging.hpp
        src/utils/env.hpp
        src/utils/random.hpp
        src/utils/compare.hpp
        src/utils/runguard.hpp

        src/handlers/c.hpp
        src/handlers/abstract.hpp
        src/handlers/cpp.hpp
        src/handlers/python3.hpp
        src/handlers/register.hpp
        src/api/info.hpp
        src/api/submit.hpp
        src/api/result.hpp
)

# Install these packages with vcpkg
find_package(Crow CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

target_link_libraries(main PRIVATE Crow::Crow asio::asio spdlog::spdlog nlohmann_json::nlohmann_json)
