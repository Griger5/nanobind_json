import pytest
import sys
import math

import test_json_ext as t

def test_nljson_none_from_json():
    json = t.nljson_none_fromjson()

    assert json == None

def test_nljson_none_to_json():
    t.nljson_none_tojson()

def test_nljson_bool_from_json():
    json = t.nljson_bool_fromjson()

    assert isinstance(json, bool)
    assert json == False

def test_nljson_bool_to_json():
    t.nljson_bool_tojson(False)

def test_nljson_integer_from_json():
    json = t.nljson_integer_fromjson()

    assert isinstance(json, int)
    assert json == 42

def test_nljson_integer_to_json():
    t.nljson_integer_tojson(42, sys.maxsize, ~sys.maxsize)

def test_nljson_floating_from_json():
    json_float, json_inf, json_nan = t.nljson_floating_fromjson()

    assert isinstance(json_float, float)
    assert json_float == 4.5

    assert isinstance(json_inf, float)
    assert math.isinf(json_inf)

    assert isinstance(json_nan, float)
    assert math.isnan(json_nan)

def test_nljson_floating_to_json():
    t.nljson_floating_tojson(4.5, math.inf, math.nan)

def test_nljson_string_from_json():
    json = t.nljson_string_fromjson()

    assert isinstance(json, str)
    assert json == "string from cpp"

def test_nljson_string_to_json():
    t.nljson_string_tojson("string from python")

def test_nljson_list_from_json():
    json = t.nljson_list_fromjson()

    assert isinstance(json, list)
    assert json[0] == 1234
    assert json[1] == "Hello, World!"
    assert json[2] == False

def test_nljson_list_to_json():
    t.nljson_list_tojson([1234, "Hello, World!", False], [])

def test_nljson_tuple_to_json():
    t.nljson_tuple_tojson((1234, "Hello, World!", False))

def test_nljson_dict_from_json():
    json = t.nljson_dict_fromjson()

    assert isinstance(json, dict)
    assert json["a"] == 1234
    assert json["b"] == "Hello, World!"
    assert json["c"] == False

def test_nljson_dict_to_json():
    t.nljson_dict_tojson({"a":1234, "b":"Hello, World!", "c":False}, {})

def test_nljson_nested_from_json():
    json = t.nljson_nested_fromjson()

    assert isinstance(json, dict)
    assert isinstance(json["baz"], list)
    assert isinstance(json["foo"], int)
    assert isinstance(json["bar"], dict)

    assert json["baz"][0] == "one"
    assert json["baz"][1] == "two"
    assert json["baz"][2] == "three"
    assert json["foo"] == 1
    assert json["bar"]["a"] == 36
    assert json["bar"]["b"] == False
    assert json["hey"] == None

def test_nljson_nested_to_json():
    json = {
        "baz": ["one", "two", "three"],
        "foo": 1,
        "bar": {"a": 36, "b": False},
        "hey": None
    }

    t.nljson_nested_tojson(json)

def test_nljson_handle_to_json():
    py_object = []
    py_object.append((1234, "abc", False))
    py_object.append({"a":3, "b":5})
    py_object.append("hello")
    py_object.append(None)

    t.nljson_handle_tojson(py_object)

def test_nljson_circular_reference():
    circular_list = []
    circular_list.append(circular_list)

    circular_dict = {}
    circular_dict["circ"] = circular_dict

    with pytest.raises(RuntimeError) as e_list:
        t.nljson_circular_reference(circular_list)

    with pytest.raises(RuntimeError) as e_dict:
        t.nljson_circular_reference(circular_dict)

    assert str(e_list.value) == "Circular reference detected"
    assert str(e_dict.value) == "Circular reference detected"

def test_nljson_ordered_from_json():
    json = t.nljson_ordered_fromjson()

    for i,key in enumerate(json.keys()):
        assert key == f"test_{2-i}"

def test_nljson_ordered_to_json():
    test_dict = {
        "test_2":"2",
        "test_1":"1",
        "test_0":"0"
    }

    t.nljson_ordered_tojson(test_dict)