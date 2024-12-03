#include <cytnx_core/test.hpp>
#include <pybind11/pybind11.h>
#include <string>

std::string hello_from_bin() { return cytnx_core::test(); }

namespace py = pybind11;

PYBIND11_MODULE(_core, m) {
  m.doc() = "pybind11 hello module";

  m.def("hello_from_bin", &hello_from_bin, R"pbdoc(
      A function that returns a Hello string.
  )pbdoc");

  py::class_<cytnx_core::MyClass>(m, "MyClass")
      // construction
      .def(py::init<int>(), py::arg("num"))
      .def("get_num", &cytnx_core::MyClass::get_num);
}
