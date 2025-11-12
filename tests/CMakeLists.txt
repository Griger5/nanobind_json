find_package(Python 3.8 COMPONENTS Interpreter Development REQUIRED)

include(FetchContent)

find_package(nanobind)
find_package(nlohmann_json)

if(NOT nanobind_FOUND)
    FetchContent_Declare(
        nanobind
        GIT_REPOSITORY https://github.com/wjakob/nanobind.git
        GIT_TAG master
    )
    FetchContent_MakeAvailable(nanobind)
    include_directories(../build/_deps/nanobind-src/include)
endif()

if(NOT nlohmann_json_FOUND)
    FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG master
    )
    FetchContent_MakeAvailable(nlohmann_json)
    include_directories(../build/_deps/nlohmann_json-src/include)
endif()

include_directories(../include)
nanobind_add_module(test_json_ext test_json.cpp STABLE_ABI)