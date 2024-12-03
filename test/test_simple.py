from cytnx_core import MyClass, hello


def test_str():
    assert isinstance(hello(), str)


def test_myclass():
    x = MyClass(num=3)
    assert x.get_num() == 3


print(hello())
