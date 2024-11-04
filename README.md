![Tests](https://github.com/nanobind/nanobind_json/workflows/Tests/badge.svg)

# nanobind11_json
`nanobind11_json` is an `nlohmann::json` to `nanobind` bridge, it allows you to automatically convert `nlohmann::json` to `py::object` and the other way around. Simply include the header, and the automatic conversion will be enabled.

This library is derived from that of `pybind11_json` and therefore follows the same BSD license, even though the first round of translations were from a U.S. government employee.

## Warnings

Due to some changes between `pybind11` and `nanobind`, some features (most notably the tests, which rely on the embed mode), have not been re-implemented. Recommend to use this library as a header-only via a git submodule for now.

## C++ API: Automatic conversion between `nlohmann::json` and `pybind11` Python objects

```cpp
#include "nanobind_json/nanobind_json.hpp"

namespace nb = nanobind;
namespace nl = nlohmann;
using namespace nanobind::literals;

py::dict obj = py::dict("number"_a=1234, "hello"_a="world");

// Automatic py::dict->nl::json conversion
nl::json j = obj;

// Automatic nl::json->py::object conversion
nb::object result1 = j;
// Automatic nl::json->py::dict conversion
nb::dict result2 = j;
```

## Making bindings

You can easily make bindings for C++ classes/functions that make use of `nlohmann::json`.

For example, making a binding for the following two functions is automatic, thanks to `nanobind_json`

```cpp
void take_json(const nlohmann::json& json) {
    std::cout << "This function took an nlohmann::json instance as argument: " << s << std::endl;
}

nlohmann::json return_json() {
    nlohmann::json j = {{"value", 1}};

    std::cout << "This function returns an nlohmann::json instance: "  << j << std::endl;

    return j;
}
```

Bindings:

```cpp
NB_MODULE(my_module, m) {
    m.doc() = "My awesome module";

    m.def("take_json", &take_json, "pass nb::object to a C++ function that takes an nlohmann::json");
    m.def("return_json", &return_json, "return nb::object from a C++ function that returns an nlohmann::json");
}
```

You can then use your functions from Python:

```python
import my_module

my_module.take_json({"value": 2})
j = my_module.return_json()

print(j)
```

# Installation

TODO: not yet implemented; PR welcome

## Using conda

You can install `nanobind_json` using conda

```bash
conda install -c conda-forge nanobind nlohmann_json nanobind_json
```

## From sources

We encourage you to use conda for installing dependencies, but it is not a requirement for `nanobind_json` to work

```bash
conda install cmake nlohmann_json nanobind -c conda-forge
```

Then you can install the sources

```bash
cmake -D CMAKE_INSTALL_PREFIX=your_conda_path
make install
```

## Header only usage
Download the "nanobind_json.hpp" single file into your project, and install/download `nanobind` and `nlohmann_json` or use as git submodule.


## Run tests

TODO: not yet implemented because nanobind has dropped embed mode; PR fixing the tests is welcomed
TODO: nanobind cmake packages are not automatically found when importing CMake

You can compile and run tests locally doing

```bash
cmake -D CMAKE_INSTALL_PREFIX=$CONDA_PREFIX -D DOWNLOAD_GTEST=ON ..
make
./test/test_nanobind_json
```

# Dependencies

``nanobind_json`` depends on

 - [nanobind](https://github.com/wjakob/nanobind)
 - [nlohmann_json](https://github.com/nlohmann/json)


| `nanobind_json`| `nlohmann_json` | `nanobind`      |
|----------------|-----------------|-----------------|
|  master        | >=3.2.0,<4.0    | >=2.2.0,<3.0    |


# License

We use a shared copyright model that enables all contributors to maintain the
copyright on their contributions.

This software is licensed under the BSD-3-Clause license. See the [LICENSE](LICENSE) file for details.
