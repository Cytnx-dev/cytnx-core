from cytnx_core import device


def test_device():
    assert isinstance(device.Cpu, int)
    assert isinstance(device.Cuda, int)
    assert isinstance(device.Ngpus, int)
    assert isinstance(device.Ncpus, int)


def test_device_prop():
    device.print_property()


def test_getname():
    name = device.getname(device.Cpu)
    assert isinstance(name, str)
