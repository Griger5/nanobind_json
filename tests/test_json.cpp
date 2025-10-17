/***************************************************************************
* Copyright (c) 2025, Gracjan Adamus                                       *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include <nanobind/nanobind.h>
#include "nanobind_json/nanobind_json.h"

namespace nb = nanobind;
namespace nl = nlohmann;

void fail() { throw std::exception(); }

NB_MODULE(test_json_ext, m) {
    m.def("nljson_none_fromjson", []() {
        return "null"_json;
    });

    m.def("nljson_none_tojson", []() {
        nb::object obj1;
        nb::object obj2 = nb::none();

        nl::json j1 = obj1;
        nl::json j2 = obj2;

        if (!j1.is_null()) {
            fail();
        }
        if (!j2.is_null()) {
            fail();
        }
    });

    m.def("nljson_bool_fromjson", []() {
        return "false"_json;
    });

    m.def("nljson_bool_tojson", [](nl::json bool_json) {
        if (!bool_json.is_boolean()) {
            fail();
        }
        else if (bool_json.get<bool>() != false) {
            fail();
        }
    });
}