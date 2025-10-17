import pytest

import test_json_ext as t

def test_nljson_none_from_json():
    json = t.nljson_none_fromjson()

    assert json == None

def test_nljson_none_to_json():
    t.nljson_none_tojson()

def test_nljson_bool_from_json():
    json = t.nljson_bool_fromjson()

    assert json == False

def test_nljson_bool_to_json():
    t.nljson_bool_tojson(False)