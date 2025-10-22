/***************************************************************************
* Copyright (c) 2025, Gracjan Adamus                                       *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include <nanobind/nanobind.h>
#include <nanobind/stl/tuple.h>

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

    m.def("nljson_integer_fromjson", []() {
        return "42"_json;
    });

    m.def("nljson_integer_tojson", [](nl::json integer_json, nl::json max_json, nl::json min_json) {
        if (!integer_json.is_number_integer()) {
            fail();
        }
        else if (integer_json.get<int>() != 42) {
            fail();
        }

        if (!max_json.is_number_integer()) {
            fail();
        }
        else if (max_json.get<nl::json::number_integer_t>() != std::numeric_limits<nl::json::number_integer_t>::max()) {
            fail();
        }

        if (!min_json.is_number_integer()) {
            fail();
        }
        else if (min_json.get<nl::json::number_integer_t>() != std::numeric_limits<nl::json::number_integer_t>::min()) {
            fail();
        }
    });

    m.def("nljson_floating_fromjson", []() {
        return std::make_tuple(nl::json(4.5), nl::json(INFINITY), nl::json(NAN));
    });

    m.def("nljson_floating_tojson", [](nl::json floating_json, nl::json inf_json, nl::json nan_json) {
        if (!floating_json.is_number_float()) {
            fail();
        }
        else if (floating_json.get<double>() != 4.5) {
            fail();
        }

        if (!inf_json.is_number_float()) {
            fail();
        }
        else if (inf_json.get<double>() != INFINITY) {
            fail();
        }

        if (!nan_json.is_number_float()) {
            fail();
        }
        else if (!isnan(nan_json.get<double>())) {
            fail();
        }
    });

    m.def("nljson_string_fromjson", []() {
        return nl::json("string from cpp");
    });

    m.def("nljson_string_tojson", [](nl::json string_json) {
        if (!string_json.is_string()) {
            fail();
        }
        else if (string_json.get<std::string>() != "string from python") {
            fail();
        }
    });
}