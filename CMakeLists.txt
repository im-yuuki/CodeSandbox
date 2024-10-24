cmake_minimum_required(VERSION 3.28)
project(CodeSandbox)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -O3 -march=native -DNDEBUG")

add_executable(CodeSandbox
        src/main.cpp

        src/data/submissions.hpp
        src/data/enums.hpp
        src/data/problems.hpp

        src/utils/logging.hpp
        src/utils/env.hpp
        src/utils/random.hpp
        src/utils/compare.hpp
        src/utils/runguard.hpp

        src/modules/c.hpp
        src/modules/abstract.hpp
        src/modules/cpp.hpp
        src/modules/register.hpp

        src/api/info.hpp
        src/api/submit.hpp

        src/data/storage.hpp
)

# Install these packages with vcpkg
find_package(Crow CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

target_link_libraries(CodeSandbox PRIVATE Crow::Crow asio::asio spdlog::spdlog nlohmann_json::nlohmann_json)
