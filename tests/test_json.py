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