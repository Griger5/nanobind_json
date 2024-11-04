/***************************************************************************
* Copyright (c) 2019, Martin Renou                                         *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include <string>
#include <vector>
#include <cmath>
#include <limits>

#include "gtest/gtest.h"

#include "nanobind_json/nanobind_json.hpp"

#include "nanobind/embed.h"

namespace nb = nanobind;
namespace nl = nlohmann;

using namespace pybind11::literals;

inline nb::module create_module(const std::string& module_name)
{
    return nb::module_::create_extension_module(module_name.c_str(), nullptr, new nb::module_::module_def);
}

TEST(nljson_serializers_tojson, none)
{
    nb::scoped_interpreter guard;
    nb::object obj1;
    nb::object obj2 = nb::none();
    nl::json j1 = obj1;
    nl::json j2 = obj2;

    ASSERT_TRUE(j1.is_null());
    ASSERT_TRUE(j2.is_null());
}

TEST(nljson_serializers_tojson, bool_)
{
    nb::scoped_interpreter guard;
    nb::bool_ obj(false);
    nl::json j = obj;

    ASSERT_TRUE(j.is_boolean());
    ASSERT_FALSE(j.get<bool>());
}

TEST(nljson_serializers_tojson, integer)
{
    nb::scoped_interpreter guard;
    nb::int_ obj(36);
    nl::json j = obj;

    ASSERT_TRUE(j.is_number_integer());
    ASSERT_EQ(j.get<int>(), 36);

    nb::int_ obj_integer_min(std::numeric_limits<nl::json::number_integer_t>::min());
    nl::json j_integer_min = obj_integer_min;

    ASSERT_TRUE(j_integer_min.is_number_integer());
    ASSERT_EQ(j_integer_min.get<nl::json::number_integer_t>(), std::numeric_limits<nl::json::number_integer_t>::min());

    nb::int_ obj_unsigned_max(std::numeric_limits<nl::json::number_unsigned_t>::max());
    nl::json j_unsigned_max = obj_unsigned_max;

    ASSERT_TRUE(j_unsigned_max.is_number_unsigned());
    ASSERT_EQ(j_unsigned_max.get<nl::json::number_unsigned_t>(), std::numeric_limits<nl::json::number_unsigned_t>::max());

    nb::int_ obj_integer_border = nb::int_(std::numeric_limits<nl::json::number_integer_t>::max()).attr("__add__")(1);
    nl::json j_integer_border = obj_integer_border;

    ASSERT_TRUE(j_integer_border.is_number_unsigned());
    ASSERT_EQ(j_integer_min.get<nl::json::number_unsigned_t>(), (nl::json::number_unsigned_t)(std::numeric_limits<nl::json::number_integer_t>::max()) + 1);

    nb::int_ obj_small_outside = obj_integer_min.attr("__sub__")(1);
    ASSERT_THROW(nl::json j_small_outside = obj_small_outside, std::runtime_error);

    nb::int_ obj_large_outside=obj_unsigned_max.attr("__add__")(1);
    ASSERT_THROW(nl::json j_large_outside = obj_large_outside, std::runtime_error);
}

TEST(nljson_serializers_tojson, float_)
{
    nb::scoped_interpreter guard;
    nb::float_ obj(36.37);
    nl::json j = obj;

    ASSERT_TRUE(j.is_number_float());
    ASSERT_EQ(j.get<double>(), 36.37);

    nb::float_ obj_inf(INFINITY);
    nl::json j_inf = obj_inf;

    ASSERT_TRUE(j_inf.is_number_float());
    ASSERT_EQ(j_inf.get<double>(), INFINITY);

    nb::float_ obj_nan(NAN);
    nl::json j_nan = obj_nan;

    ASSERT_TRUE(j_nan.is_number_float());
    ASSERT_TRUE(isnan(j_nan.get<double>()));
}

TEST(nljson_serializers_tojson, string)
{
    nb::scoped_interpreter guard;
    nb::str obj("Hello");
    nl::json j = obj;

    ASSERT_TRUE(j.is_string());
    ASSERT_EQ(j.get<std::string>(), "Hello");
}

TEST(nljson_serializers_tojson, list)
{
    nb::scoped_interpreter guard;
    nb::list obj;
    obj.append(nb::int_(36));
    obj.append(nb::str("Hello World"));
    obj.append(nb::bool_(false));
    nl::json j = obj;

    ASSERT_TRUE(j.is_array());
    ASSERT_EQ(j[0].get<int>(), 36);
    ASSERT_EQ(j[1].get<std::string>(), "Hello World");
    ASSERT_EQ(j[2].get<bool>(), false);
}

TEST(nljson_serializers_tojson, empty_list)
{
    nb::scoped_interpreter guard;
    nb::list obj;
    nl::json j = obj;

    ASSERT_TRUE(j.is_array());
    ASSERT_TRUE(j.empty());
}

TEST(nljson_serializers_tojson, tuple)
{
    nb::scoped_interpreter guard;
    nb::tuple obj = nb::make_tuple(1234, "hello", false);
    nl::json j = obj;

    ASSERT_TRUE(j.is_array());
    ASSERT_EQ(j[0].get<int>(), 1234);
    ASSERT_EQ(j[1].get<std::string>(), "hello");
    ASSERT_EQ(j[2].get<bool>(), false);
}

TEST(nljson_serializers_tojson, dict)
{
    nb::scoped_interpreter guard;
    nb::dict obj("number"_a=1234, "hello"_a="world");
    nl::json j = obj;

    ASSERT_TRUE(j.is_object());
    ASSERT_EQ(j["number"].get<int>(), 1234);
    ASSERT_EQ(j["hello"].get<std::string>(), "world");
}

TEST(nljson_serializers_tojson, empty_dict)
{
    nb::scoped_interpreter guard;
    nb::dict obj;
    nl::json j = obj;

    ASSERT_TRUE(j.is_object());
    ASSERT_TRUE(j.empty());
}

TEST(nljson_serializers_tojson, nested)
{
    nb::scoped_interpreter guard;
    nb::dict obj(
        "list"_a=nb::make_tuple(1234, "hello", false),
        "dict"_a=nb::dict("a"_a=12, "b"_a=13),
        "hello"_a="world",
        "world"_a=nb::none()
    );
    nl::json j = obj;

    ASSERT_TRUE(j.is_object());
    ASSERT_EQ(j["list"][0].get<int>(), 1234);
    ASSERT_EQ(j["list"][1].get<std::string>(), "hello");
    ASSERT_EQ(j["list"][2].get<bool>(), false);
    ASSERT_EQ(j["dict"]["a"].get<int>(), 12);
    ASSERT_EQ(j["dict"]["b"].get<int>(), 13);
    ASSERT_EQ(j["world"], nullptr);
}

TEST(nljson_serializers_tojson, handle)
{
    nb::scoped_interpreter guard;
    nb::list obj;
    obj.append(nb::make_tuple(1234, "hello", false));
    obj.append(nb::dict("a"_a=12, "b"_a=13));
    obj.append("world");
    obj.append(nb::none());

    for (nb::handle handle : obj)
    {
        nl::json j = handle;

        ASSERT_TRUE(j.is_array() || j.is_object() || j.is_string() || j.is_null());
    }
}

TEST(nljson_serializers_tojson, list_accessor)
{
    nb::scoped_interpreter guard;
    nb::list obj;
    obj.append(nb::make_tuple(1234, "hello", false));
    obj.append(nb::dict("a"_a=12, "b"_a=13));
    obj.append("world");
    obj.append(nb::none());

    nl::json j = obj[0];
    ASSERT_TRUE(j.is_array());

    j = obj[1];
    ASSERT_TRUE(j.is_object());

    j = nb::make_tuple(1234, "hello", false)[0];
    ASSERT_TRUE(j.is_number_integer());
    ASSERT_EQ(j.get<int>(), 1234);
}

TEST(nljson_serializers_tojson, tuple_accessor)
{
    nb::scoped_interpreter guard;
    nb::tuple obj = nb::make_tuple(1234, "hello", false);

    nl::json j = obj[0];
    ASSERT_TRUE(j.is_number_integer());
    ASSERT_EQ(j.get<int>(), 1234);

    j = obj[1];
    ASSERT_TRUE(j.is_string());
    ASSERT_EQ(j.get<std::string>(), "hello");
}

TEST(nljson_serializers_tojson, item_accessor)
{
    nb::scoped_interpreter guard;
    nb::dict obj = nb::dict("a"_a=12, "b"_a="hello");

    nl::json j = obj["a"];
    ASSERT_TRUE(j.is_number());
    ASSERT_EQ(j.get<int>(), 12);

    j = obj["b"];
    ASSERT_TRUE(j.is_string());
    ASSERT_EQ(j.get<std::string>(), "hello");
}

TEST(nljson_serializers_tojson, str_attr_accessor)
{
    nb::scoped_interpreter guard;
    nb::module sys = nb::module::import("sys");

    nl::json j = sys.attr("base_prefix");
    ASSERT_TRUE(j.is_string());
}

TEST(nljson_serializers_tojson, obj_attr_accessor)
{
    nb::scoped_interpreter guard;
    nb::module sys = nb::module::import("sys");

    nb::str base_prefix = "base_prefix";

    nl::json j = sys.attr(base_prefix);
    ASSERT_TRUE(j.is_string());
}

TEST(nljson_serializers_fromjson, none)
{
    nb::scoped_interpreter guard;
    nl::json j = "null"_json;
    nb::object obj = j;

    ASSERT_TRUE(obj.is_none());
}

TEST(nljson_serializers_fromjson, bool_)
{
    nb::scoped_interpreter guard;
    nl::json j = "false"_json;
    nb::object obj = j;

    ASSERT_TRUE(nb::isinstance<nb::bool_>(obj));
    ASSERT_FALSE(obj.cast<bool>());

    nb::bool_ obj2 = j;

    ASSERT_FALSE(obj2.cast<bool>());
}

TEST(nljson_serializers_fromjson, integer)
{
    nb::scoped_interpreter guard;
    nl::json j = "36"_json;
    nb::object obj = j;

    ASSERT_TRUE(nb::isinstance<nb::int_>(obj));
    ASSERT_EQ(obj.cast<int>(), 36);

    nb::int_ obj2 = j;

    ASSERT_EQ(obj2.cast<int>(), 36);
}

TEST(nljson_serializers_fromjson, integer_large_unsigned)
{
    // Note: if the asserts below error, the large number is printed as "-1" with
    // an overflow error. This is only in the output step in pybind. Calling
    // nb::print on the objects shows the correct large unsigned integer.

    nb::scoped_interpreter guard;
    uint64_t original = 13625394757606569013ull;
    nb::int_ py_orig = original;
    nl::json j = original;
    nb::object obj = j;

    ASSERT_TRUE(nb::isinstance<nb::int_>(obj));
    ASSERT_EQ(obj.cast<uint64_t>(), original);

    nb::int_ obj2 = j;

    // Use .equal to compare values not pointers
    ASSERT_TRUE(obj2.equal(py_orig));
}

TEST(nljson_serializers_fromjson, float_)
{
    nb::scoped_interpreter guard;
    nl::json j = "36.2"_json;
    nb::object obj = j;

    ASSERT_TRUE(nb::isinstance<nb::float_>(obj));
    ASSERT_EQ(obj.cast<double>(), 36.2);

    nb::float_ f_obj = j;

    ASSERT_EQ(f_obj.cast<double>(), 36.2);

    nl::json j_inf(INFINITY);
    nb::object obj_inf = j_inf;

    ASSERT_TRUE(nb::isinstance<nb::float_>(obj_inf));
    ASSERT_EQ(obj_inf.cast<double>(), INFINITY);

    nb::float_ f_obj_inf = j_inf;

    ASSERT_EQ(f_obj_inf.cast<double>(), INFINITY);

    nl::json j_nan(NAN);
    nb::object obj_nan = j_nan;

    ASSERT_TRUE(nb::isinstance<nb::float_>(obj_nan));
    ASSERT_TRUE(isnan(obj_nan.cast<double>()));

    nb::float_ f_obj_nan = j_nan;

    ASSERT_TRUE(isnan(f_obj_nan.cast<double>()));
}

TEST(nljson_serializers_fromjson, string)
{
    nb::scoped_interpreter guard;
    nl::json j = "\"Hello World!\""_json;
    nb::object obj = j;

    ASSERT_TRUE(nb::isinstance<nb::str>(obj));
    ASSERT_EQ(obj.cast<std::string>(), "Hello World!");

    nb::str obj2 = j;

    ASSERT_EQ(obj2.cast<std::string>(), "Hello World!");
}

TEST(nljson_serializers_fromjson, list)
{
    nb::scoped_interpreter guard;
    nl::json j = "[1234, \"Hello World!\", false]"_json;
    nb::object obj = j;

    ASSERT_TRUE(nb::isinstance<nb::list>(obj));
    ASSERT_EQ(nb::list(obj)[0].cast<int>(), 1234);
    ASSERT_EQ(nb::list(obj)[1].cast<std::string>(), "Hello World!");
    ASSERT_EQ(nb::list(obj)[2].cast<bool>(), false);

    nb::list obj2 = j;

    ASSERT_EQ(nb::list(obj2)[0].cast<int>(), 1234);
    ASSERT_EQ(nb::list(obj2)[1].cast<std::string>(), "Hello World!");
    ASSERT_EQ(nb::list(obj2)[2].cast<bool>(), false);
}

TEST(nljson_serializers_fromjson, dict)
{
    nb::scoped_interpreter guard;
    nl::json j = "{\"a\": 1234, \"b\":\"Hello World!\", \"c\":false}"_json;
    nb::object obj = j;

    ASSERT_TRUE(nb::isinstance<nb::dict>(obj));
    ASSERT_EQ(nb::dict(obj)["a"].cast<int>(), 1234);
    ASSERT_EQ(nb::dict(obj)["b"].cast<std::string>(), "Hello World!");
    ASSERT_EQ(nb::dict(obj)["c"].cast<bool>(), false);

    nb::dict obj2 = j;

    ASSERT_EQ(nb::dict(obj2)["a"].cast<int>(), 1234);
    ASSERT_EQ(nb::dict(obj2)["b"].cast<std::string>(), "Hello World!");
    ASSERT_EQ(nb::dict(obj2)["c"].cast<bool>(), false);
}

TEST(nljson_serializers_fromjson, nested)
{
    nb::scoped_interpreter guard;
    nl::json j = R"({
        "baz": ["one", "two", "three"],
        "foo": 1,
        "bar": {"a": 36, "b": false},
        "hey": null
    })"_json;
    nb::object obj = j;

    ASSERT_TRUE(nb::isinstance<nb::dict>(obj));
    ASSERT_TRUE(nb::isinstance<nb::list>(nb::dict(obj)["baz"]));
    ASSERT_TRUE(nb::isinstance<nb::int_>(nb::dict(obj)["foo"]));
    nb::list baz = nb::dict(obj)["baz"];
    nb::int_ foo = nb::dict(obj)["foo"];
    nb::dict bar = nb::dict(obj)["bar"];
    ASSERT_EQ(baz[0].cast<std::string>(), "one");
    ASSERT_EQ(baz[1].cast<std::string>(), "two");
    ASSERT_EQ(baz[2].cast<std::string>(), "three");
    ASSERT_EQ(foo.cast<int>(), 1);
    ASSERT_EQ(bar["a"].cast<int>(), 36);
    ASSERT_FALSE(bar["b"].cast<bool>());
    ASSERT_TRUE(nb::dict(obj)["hey"].is_none());
}

inline const nl::json& test_fromtojson(const nl::json& json)
{
    return json;
}

TEST(pybind11_caster_tojson, dict)
{
    nb::scoped_interpreter guard;
    nb::module m = create_module("test");

    m.def("to_json", &test_fromtojson);

    // Simulate calling this binding from Python with a dictionary as argument
    nb::dict obj("number"_a=1234, "hello"_a="world");
    nl::json j = m.attr("to_json")(obj);

    ASSERT_TRUE(j.is_object());
    ASSERT_EQ(j["number"].get<int>(), 1234);
    ASSERT_EQ(j["hello"].get<std::string>(), "world");
}

TEST(pybind11_caster_fromjson, dict)
{
    nb::scoped_interpreter guard;
    nb::module m = create_module("test");

    m.def("from_json", &test_fromtojson);

    // Simulate calling this binding from Python, getting back a nb::object
    nb::dict obj("number"_a=1234, "hello"_a="world");
    nb::dict j = m.attr("from_json")(obj);

    ASSERT_EQ(j["number"].cast<int>(), 1234);
    ASSERT_EQ(j["hello"].cast<std::string>(), "world");
}
