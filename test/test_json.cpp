/***************************************************************************
* Copyright (c) 2025, Gracjan Adamus                                       *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nlohmann/json.hpp>
#include "../include/nanobind_json/nanobind_json.h"

#include <iostream>

namespace nb = nanobind;
namespace nl = nlohmann;

void fail() { throw std::exception(); }

NB_MODULE(test_json_ext, m) {
    m.def("nljson_bool_fromjson", []() {
        return "false"_json;
    });

    m.def("nljson_bool_tojson", [](nb::bool_ bool_) {
        nl::json bool_json = bool_;
        if (!bool_json.is_boolean()) {
            fail();
        }
        else if (bool_json.get<bool>() != false) {
            fail();
        }
    });
}