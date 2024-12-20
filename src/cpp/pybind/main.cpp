#include <map>
#include <random>
#include <vector>

#include <pybind11/buffer_info.h>
#include <pybind11/functional.h>
#include <pybind11/iostream.h>
#include <pybind11/numpy.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "complex.h"
#include <cytnx_core/cytnx_core.hpp>

namespace py = pybind11;
using namespace pybind11::literals;
using namespace cytnx_core;

// ref:
// https://developer.lsst.io/v/DM-9089/coding/python_wrappers_for_cpp_with_pybind11.html
// ref: https://pybind11.readthedocs.io/en/stable/advanced/classes.html
// ref:
// https://block.arch.ethz.ch/blog/2016/07/adding-methods-to-python-classes/
// ref:
// https://medium.com/@mgarod/dynamically-add-a-method-to-a-class-in-python-c49204b85bd6
// void bond_binding(py::module &m);
// void symmetry_binding(py::module &m);

// void generator_binding(py::module &m);
// void storage_binding(py::module &m);
// void tensor_binding(py::module &m);

// void network_binding(py::module &m);

// class PyLinOp;
// void linop_binding(py::module &m);

// class cHclass;
// void unitensor_binding(py::module &m);

// void linalg_binding(py::module &m);
// void algo_binding(py::module &m);
// void physics_related_binding(py::module &m);
// void random_binding(py::module &m);
// void tnalgo_binding(py::module &m);
// void scalar_binding(py::module &m);

// void ncon_binding(py::module &m);

PYBIND11_MODULE(_core, m) {
  // m.attr("__blasINTsize__") = cytnx_core::__blasINTsize__;

  py::add_ostream_redirect(m, "ostream_redirect");

  py::enum_<cytnx_core::Type_class::Type> type_enum(m, "Type");
  for (std::size_t i = 0; i < N_Type; ++i) {
    type_enum.value(Type.enum_name(i), static_cast<Type_class::Type>(i));
  }
  type_enum.export_values();

  auto mdev = m.def_submodule("device");
  mdev.attr("Cpu") = (cytnx_int64)cytnx_core::Device.cpu;
  mdev.attr("Cuda") = (cytnx_int64)cytnx_core::Device.cuda;
  mdev.attr("Ngpus") = cytnx_core::Device.Ngpus;
  mdev.attr("Ncpus") = cytnx_core::Device.Ncpus;
  mdev.def("print_property", []() { cytnx_core::Device.print_property(); });
  mdev.def("getname", [](const int &device_id) -> std::string {
    return cytnx_core::Device.getname(device_id);
  });

  // m.def("set_mkl_ilp64", &cytnx_core::set_mkl_ilp64);
  // m.def("get_mkl_code", &cytnx_core::get_mkl_code);

  // global vars
  // m.attr("cytnxdevice") = cytnx_core::cytnxdevice;
  // m.attr("Type")   = py::cast(cytnx_core::Type);
  // m.attr("redirect_output") = py::capsule(new
  // py::scoped_ostream_redirect(...),
  //[](void *sor) { delete static_cast<py::scoped_ostream_redirect *>(sor); });

  // py::enum_<cytnx_core::__device::__pybind_device>(m,"Device",py::arithmetic())
  //     .value("cpu", cytnx_core::__device::__pybind_device::cpu)
  //	.value("cuda", cytnx_core::__device::__pybind_device::cuda)
  //	.export_values();

  // m.attr("Device") = py::module::import("enum").attr("IntEnum")
  //     ("Device", py::dict("cpu"_a=(cytnx_int64)cytnx_core::Device.cpu,
  //     "cuda"_a=(cytnx_int64)cytnx_core::Device.cuda));

  // m.def(
  //   "ncon",
  //   [](const std::vector<UniTensor> &tensor_list_in,
  //       const std::vector<std::vector<cytnx_int64>> &connect_list_in,
  //       const bool check_network, const bool optimize,
  //       std::vector<cytnx_int64> cont_order ,
  //       const std::vector<std::string> &out_labels) -> UniTensor {
  //     return ncon(tensor_list_in, connect_list_in, check_network, optimize,
  //     cont_order, out_labels);
  //   },
  //   py::arg("tensor_list_in")= std::vector<UniTensor>(),
  //   py::arg("connect_list_in")= std::vector<std::vector<cytnx_int64>>(),
  //   py::arg("check_network")= false,
  //   py::arg("optimize") = false,
  //   py::arg("cont_order") = std::vector<cytnx_int64>(),
  //   py::arg("out_labels") = std::vector<std::string>());

  // generator_binding(m);
  // scalar_binding(m);
  // storage_binding(m);
  // tensor_binding(m);
  // network_binding(m);
  // linop_binding(m);
  // unitensor_binding(m);
  // linalg_binding(m);
  // algo_binding(m);
  // physics_related_binding(m);
  // random_binding(m);
  // tnalgo_binding(m);
  // ncon_binding(m);
}
