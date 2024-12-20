from cytnx_core import Type


def test_type():
    assert isinstance(Type.Void, Type)
    assert isinstance(Type.Bool, Type)
    assert isinstance(Type.Uint16, Type)
    assert isinstance(Type.Int16, Type)
    assert isinstance(Type.Uint32, Type)
    assert isinstance(Type.Int32, Type)
    assert isinstance(Type.Uint64, Type)
    assert isinstance(Type.Int64, Type)
    assert isinstance(Type.Float, Type)
    assert isinstance(Type.Double, Type)
    assert isinstance(Type.ComplexFloat, Type)
    assert isinstance(Type.ComplexDouble, Type)
