#include <cytnx_core/test.hpp>
#include <pybind11/pybind11.h>
#include <string>

std::string hello_from_bin() { return cytnx_core::test(); }

namespace py = pybind11;

PYBIND11_MODULE(pycytnx, m) {
  m.doc() = "pybind11 hello module";

  m.def("hello_from_bin", &hello_from_bin, R"pbdoc(
      A function that returns a Hello string.
  )pbdoc");
}
