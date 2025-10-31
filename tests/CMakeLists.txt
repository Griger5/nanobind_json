include(FetchContent)
        
FetchContent_Declare(
    nanobind
    GIT_REPOSITORY https://github.com/wjakob/nanobind.git
    GIT_TAG master
)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG master
)

FetchContent_MakeAvailable(nanobind nlohmann_json)

include_directories(../build/_deps/nlohmann_json-src/include)

include_directories(../include)
nanobind_add_module(test_json_ext test_json.cpp STABLE_ABI)