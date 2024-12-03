#include <cytnx_core/test.hpp>
#include <iostream>
#include <string>

namespace cytnx_core {
    std::string test() { return "Hello from test"; }

    MyClass::MyClass(int num) : num(num) {}
    int MyClass::get_num() { return this->num; }

} // namespace cytnx_core
