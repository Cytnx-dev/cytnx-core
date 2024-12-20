#ifndef CYTNX_CORE_STORAGE_IMPL_H_
#define CYTNX_CORE_STORAGE_IMPL_H_

#include <cstdlib>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>
#include <memory>

// #include "boost/smart_ptr/intrusive_ptr.hpp"

// #include "backend/Scalar.hpp"
#include <cytnx_core/errors/cytnx_error.hpp>
#include <cytnx_core/Device.hpp>
// #include "intrusive_ptr_base.hpp"
#include <cytnx_core/Type.hpp>

#define STORAGE_DEFT_SZ 2

namespace cytnx_core {

  ///@cond
  class Storage_base : public std::shared_ptr<Storage_base> {
   public:
    void *Mem;
    // std::vector<unsigned int> shape;

    unsigned long long len;  // default 0
    unsigned long long cap;  // default 0
    unsigned int dtype;  // default 0, Void
    int device;  // default -1, on cpu

    Storage_base() : cap(0), len(0), Mem(NULL), dtype(0), device(-1) {};
    // Storage_base(const std::initializer_list<unsigned int> &init_shape);
    // Storage_base(const std::vector<unsigned int> &init_shape);
    Storage_base(const unsigned long long &len_in, const int &device, const bool &init_zero = true);

    Storage_base(Storage_base &Rhs);
    Storage_base &operator=(Storage_base &Rhs);
    std::shared_ptr<Storage_base> astype(const unsigned int &dtype);

    // void Init(const std::initializer_list<unsigned int> &init_shape);
    std::string dtype_str() const;
    std::string device_str() const;
    const unsigned long long &capacity() const { return this->cap; }
    const unsigned long long &size() const { return this->len; }
    ~Storage_base();

    template <class T>
    T &at(const cytnx_uint64 &idx) const;

    template <class T>
    T &back() const;

    template <class T>
    T *data() const;

    void *data() const { return this->Mem; }

    void _cpy_bool(void *ptr, const std::vector<cytnx_bool> &vin);

    void print();
    void print_info();
    /*
        This function is design to check the type mismatch.
        Handy for developer to exclude the assign of double
        C pointer into a non-DoubleStorage.

        For example:
        float *cptr = (float*)calloc(4,sizeof(float));

        intrusive_ptr<Storage> array(new DoubleStorage());
        array->_Init_byptr((void*)cptr,4); // This is fatal, since we alloc cptr as float,
                                           // but apon free, DoubleStorage will free 2x
                                           // of memory!!!!

        array->_Init_byptr_safe(cptr,4);   // This is design to avoid the above problem
                                           // by checking the type of input pointer with
                                           // the type of Storage before call _Init_byptr.
                                           // [Note] this will intorduce overhead!!.

    */
    template <class T>
    void _Init_byptr_safe(T *rawptr, const unsigned long long &len_in) {
      // check:
      if (this->dtype == Type.Float) {
        cytnx_error_msg(typeid(T) != typeid(cytnx_float), "%s",
                        "[ERROR _Init_byptr_safe type not match]");
      } else if (this->dtype == Type.Double) {
        cytnx_error_msg(typeid(T) != typeid(cytnx_double), "%s",
                        "[ERROR _Init_byptr_safe type not match]");
      } else if (this->dtype == Type.Uint64) {
        cytnx_error_msg(typeid(T) != typeid(cytnx_uint64), "%s",
                        "[ERROR _Init_byptr_safe type not match]");
      } else if (this->dtype == Type.Uint32) {
        cytnx_error_msg(typeid(T) != typeid(cytnx_uint32), "%s",
                        "[ERROR _Init_byptr_safe type not match]");
      } else if (this->dtype == Type.Int64) {
        cytnx_error_msg(typeid(T) != typeid(cytnx_int64), "%s",
                        "[ERROR _Init_byptr_safe type not match]");
      } else if (this->dtype == Type.Int32) {
        cytnx_error_msg(typeid(T) != typeid(cytnx_int32), "%s",
                        "[ERROR _Init_byptr_safe type not match]");
      } else if (this->dtype == Type.ComplexDouble) {
        cytnx_error_msg(typeid(T) != typeid(cytnx_complex128), "%s",
                        "[ERROR _Init_byptr_safe type not match]");
      } else if (this->dtype == Type.ComplexFloat) {
        cytnx_error_msg(typeid(T) != typeid(cytnx_complex64), "%s",
                        "[ERROR _Init_byptr_safe type not match]");
      } else if (this->dtype == Type.Int16) {
        cytnx_error_msg(typeid(T) != typeid(cytnx_int16), "%s",
                        "[ERROR _Init_byptr_safe type not match]");
      } else if (this->dtype == Type.Uint16) {
        cytnx_error_msg(typeid(T) != typeid(cytnx_uint16), "%s",
                        "[ERROR _Init_byptr_safe type not match]");
      } else if (this->dtype == Type.Bool) {
        cytnx_error_msg(typeid(T) != typeid(cytnx_bool), "%s",
                        "[ERROR _Init_byptr_safe type not match]");
      } else {
        cytnx_error_msg(1, "[FATAL] ERROR%s", "\n");
      }

      this->_Init_byptr((void *)rawptr, len_in);
    }

    void GetElem_byShape_v2(std::shared_ptr<Storage_base> &out,
                            const std::vector<cytnx_uint64> &shape,
                            const std::vector<std::vector<cytnx_uint64>> &locators,
                            const cytnx_uint64 &Nunit);
    void GetElem_byShape(std::shared_ptr<Storage_base> &out, const std::vector<cytnx_uint64> &shape,
                         const std::vector<cytnx_uint64> &mapper,
                         const std::vector<cytnx_uint64> &len,
                         const std::vector<std::vector<cytnx_uint64>> &locators);
    void SetElem_byShape(std::shared_ptr<Storage_base> &in, const std::vector<cytnx_uint64> &shape,
                         const std::vector<cytnx_uint64> &mapper,
                         const std::vector<cytnx_uint64> &len,
                         const std::vector<std::vector<cytnx_uint64>> &locators,
                         const bool &is_scalar);
    void SetElem_byShape_v2(std::shared_ptr<Storage_base> &in,
                            const std::vector<cytnx_uint64> &shape,
                            const std::vector<std::vector<cytnx_uint64>> &locators,
                            const cytnx_uint64 &Nunit, const bool &is_scalar);
    // these is the one that do the work, and customize with Storage_base
    // virtual void Init(const std::vector<unsigned int> &init_shape);
    virtual void Init(const unsigned long long &len_in, const int &device = -1,
                      const bool &init_zero = true);
    virtual void _Init_byptr(void *rawptr, const unsigned long long &len_in, const int &device = -1,
                             const bool &iscap = false, const unsigned long long &cap_in = 0);

    // this function will return a new storage with the same type as the one
    // that initiate this function.
    virtual std::shared_ptr<Storage_base> _create_new_sametype();

    // [future] this will move the memory to device / cpu
    virtual void to_(const int &device);
    virtual std::shared_ptr<Storage_base> to(const int &device);

    virtual std::shared_ptr<Storage_base> clone();

    // this will perform permute on the underlying memory.
    virtual std::shared_ptr<Storage_base> Move_memory(const std::vector<cytnx_uint64> &old_shape,
                                                      const std::vector<cytnx_uint64> &mapper,
                                                      const std::vector<cytnx_uint64> &invmapper);
    virtual void Move_memory_(const std::vector<cytnx_uint64> &old_shape,
                              const std::vector<cytnx_uint64> &mapper,
                              const std::vector<cytnx_uint64> &invmapper);
    virtual void PrintElem_byShape(std::ostream &os, const std::vector<cytnx_uint64> &shape,
                                   const std::vector<cytnx_uint64> &mapper = {});
    virtual void print_elems();

    virtual std::shared_ptr<Storage_base> real();
    virtual std::shared_ptr<Storage_base> imag();

    // generators:
    virtual void fill(const cytnx_complex128 &val);
    virtual void fill(const cytnx_complex64 &val);
    virtual void fill(const cytnx_double &val);
    virtual void fill(const cytnx_float &val);
    virtual void fill(const cytnx_int64 &val);
    virtual void fill(const cytnx_uint64 &val);
    virtual void fill(const cytnx_int32 &val);
    virtual void fill(const cytnx_uint32 &val);
    virtual void fill(const cytnx_int16 &val);
    virtual void fill(const cytnx_uint16 &val);
    virtual void fill(const cytnx_bool &val);
    virtual void set_zeros();
    virtual void resize(const cytnx_uint64 &newsize);

    // virtual void append(const Scalar &val);
    virtual void append(const cytnx_complex128 &val);
    virtual void append(const cytnx_complex64 &val);
    virtual void append(const cytnx_double &val);
    virtual void append(const cytnx_float &val);
    virtual void append(const cytnx_int64 &val);
    virtual void append(const cytnx_uint64 &val);
    virtual void append(const cytnx_int32 &val);
    virtual void append(const cytnx_uint32 &val);
    virtual void append(const cytnx_int16 &val);
    virtual void append(const cytnx_uint16 &val);
    virtual void append(const cytnx_bool &val);

    // virtual Scalar get_item(const cytnx_uint64 &in) const;

    // virtual void set_item(const cytnx_uint64 &idx, const Scalar &val);
    virtual void set_item(const cytnx_uint64 &idx, const cytnx_complex128 &val);
    virtual void set_item(const cytnx_uint64 &idx, const cytnx_complex64 &val);
    virtual void set_item(const cytnx_uint64 &idx, const cytnx_double &val);
    virtual void set_item(const cytnx_uint64 &idx, const cytnx_float &val);
    virtual void set_item(const cytnx_uint64 &idx, const cytnx_int64 &val);
    virtual void set_item(const cytnx_uint64 &idx, const cytnx_uint64 &val);
    virtual void set_item(const cytnx_uint64 &idx, const cytnx_int32 &val);
    virtual void set_item(const cytnx_uint64 &idx, const cytnx_uint32 &val);
    virtual void set_item(const cytnx_uint64 &idx, const cytnx_int16 &val);
    virtual void set_item(const cytnx_uint64 &idx, const cytnx_uint16 &val);
    virtual void set_item(const cytnx_uint64 &idx, const cytnx_bool &val);

    // virtual bool approx_eq(const std::shared_ptr<Storage_base> &rhs,
    //                        const cytnx_double tol = 1e-8);
  };
  ///@endcond

  ///@cond
  template <typename DType>
  class StorageImplementation : public Storage_base {
   public:
    StorageImplementation() { this->dtype = Type.cy_typeid(DType()); };
    void Init(const unsigned long long &len_in, const int &device = -1,
              const bool &init_zero = true);
    void _Init_byptr(void *rawptr, const unsigned long long &len_in, const int &device = -1,
                     const bool &iscap = false, const unsigned long long &cap_in = 0);
    std::shared_ptr<Storage_base> _create_new_sametype();
    std::shared_ptr<Storage_base> clone();
    std::shared_ptr<Storage_base> Move_memory(const std::vector<cytnx_uint64> &old_shape,
                                              const std::vector<cytnx_uint64> &mapper,
                                              const std::vector<cytnx_uint64> &invmapper);
    void Move_memory_(const std::vector<cytnx_uint64> &old_shape,
                      const std::vector<cytnx_uint64> &mapper,
                      const std::vector<cytnx_uint64> &invmapper);
    void to_(const int &device);
    std::shared_ptr<Storage_base> to(const int &device);
    void PrintElem_byShape(std::ostream &os, const std::vector<cytnx_uint64> &shape,
                           const std::vector<cytnx_uint64> &mapper = {});
    void print_elems();

    std::shared_ptr<Storage_base> real();
    std::shared_ptr<Storage_base> imag();

    // generators:
    void fill(const cytnx_complex128 &val);
    void fill(const cytnx_complex64 &val);
    void fill(const cytnx_double &val);
    void fill(const cytnx_float &val);
    void fill(const cytnx_int64 &val);
    void fill(const cytnx_uint64 &val);
    void fill(const cytnx_int32 &val);
    void fill(const cytnx_uint32 &val);
    void fill(const cytnx_int16 &val);
    void fill(const cytnx_uint16 &val);
    void fill(const cytnx_bool &val);
    void set_zeros();
    void resize(const cytnx_uint64 &newsize);

    // void append(const Scalar &val);
    void append(const cytnx_complex128 &val);
    void append(const cytnx_complex64 &val);
    void append(const cytnx_double &val);
    void append(const cytnx_float &val);
    void append(const cytnx_int64 &val);
    void append(const cytnx_uint64 &val);
    void append(const cytnx_int32 &val);
    void append(const cytnx_uint32 &val);
    void append(const cytnx_int16 &val);
    void append(const cytnx_uint16 &val);
    void append(const cytnx_bool &val);
    // Scalar get_item(const cytnx_uint64 &in) const;

    // void set_item(const cytnx_uint64 &idx, const Scalar &val);
    void set_item(const cytnx_uint64 &idx, const cytnx_complex128 &val);
    void set_item(const cytnx_uint64 &idx, const cytnx_complex64 &val);
    void set_item(const cytnx_uint64 &idx, const cytnx_double &val);
    void set_item(const cytnx_uint64 &idx, const cytnx_float &val);
    void set_item(const cytnx_uint64 &idx, const cytnx_int64 &val);
    void set_item(const cytnx_uint64 &idx, const cytnx_uint64 &val);
    void set_item(const cytnx_uint64 &idx, const cytnx_int32 &val);
    void set_item(const cytnx_uint64 &idx, const cytnx_uint32 &val);
    void set_item(const cytnx_uint64 &idx, const cytnx_int16 &val);
    void set_item(const cytnx_uint64 &idx, const cytnx_uint16 &val);
    void set_item(const cytnx_uint64 &idx, const cytnx_bool &val);

   private:
    template <typename OtherDType>
    void Fill(const OtherDType &value);

    template <typename OtherDType>
    void Append(const OtherDType &value);
    // void Append(const Scalar &value);

    template <typename OtherDType>
    void SetItem(cytnx_uint64 index, const OtherDType &value);
    // void SetItem(cytnx_uint64 index, const Scalar &value);
  };
  ///@endcond

  using ComplexDoubleStorage = StorageImplementation<cytnx_complex128>;
  using ComplexFloatStorage = StorageImplementation<cytnx_complex64>;
  using DoubleStorage = StorageImplementation<cytnx_double>;
  using FloatStorage = StorageImplementation<cytnx_float>;
  using Uint64Storage = StorageImplementation<cytnx_uint64>;
  using Int64Storage = StorageImplementation<cytnx_int64>;
  using Uint32Storage = StorageImplementation<cytnx_uint32>;
  using Int32Storage = StorageImplementation<cytnx_int32>;
  using Uint16Storage = StorageImplementation<cytnx_uint16>;
  using Int16Storage = StorageImplementation<cytnx_int16>;
  using BoolStorage = StorageImplementation<cytnx_bool>;

  ///@cond
  typedef std::shared_ptr<Storage_base> (*pStorage_init)();
  inline std::shared_ptr<Storage_base> SIInit_cd() {
    std::shared_ptr<Storage_base> out(new ComplexDoubleStorage());
    return out;
  }
  inline std::shared_ptr<Storage_base> SIInit_cf() {
    std::shared_ptr<Storage_base> out(new ComplexFloatStorage());
    return out;
  }
  inline std::shared_ptr<Storage_base> SIInit_d() {
    std::shared_ptr<Storage_base> out(new DoubleStorage());
    return out;
  }
  inline std::shared_ptr<Storage_base> SIInit_f() {
    std::shared_ptr<Storage_base> out(new FloatStorage());
    return out;
  }
  inline std::shared_ptr<Storage_base> SIInit_u64() {
    std::shared_ptr<Storage_base> out(new Uint64Storage());
    return out;
  }
  inline std::shared_ptr<Storage_base> SIInit_i64() {
    std::shared_ptr<Storage_base> out(new Int64Storage());
    return out;
  }
  inline std::shared_ptr<Storage_base> SIInit_u32() {
    std::shared_ptr<Storage_base> out(new Uint32Storage());
    return out;
  }
  inline std::shared_ptr<Storage_base> SIInit_i32() {
    std::shared_ptr<Storage_base> out(new Int32Storage());
    return out;
  }
  inline std::shared_ptr<Storage_base> SIInit_u16() {
    std::shared_ptr<Storage_base> out(new Uint16Storage());
    return out;
  }
  inline std::shared_ptr<Storage_base> SIInit_i16() {
    std::shared_ptr<Storage_base> out(new Int16Storage());
    return out;
  }
  inline std::shared_ptr<Storage_base> SIInit_b() {
    std::shared_ptr<Storage_base> out(new BoolStorage());
    return out;
  }
  ///@endcond

  ///@cond
  class Storage_init_interface : public Type_class {
   public:
    // std::vector<pStorage_init> USIInit;
    inline static pStorage_init USIInit[N_Type];
    inline static bool inited = false;
    Storage_init_interface() {
      if (!inited) {
        USIInit[this->Double] = SIInit_d;
        USIInit[this->Float] = SIInit_f;
        USIInit[this->ComplexDouble] = SIInit_cd;
        USIInit[this->ComplexFloat] = SIInit_cf;
        USIInit[this->Uint64] = SIInit_u64;
        USIInit[this->Int64] = SIInit_i64;
        USIInit[this->Uint32] = SIInit_u32;
        USIInit[this->Int32] = SIInit_i32;
        USIInit[this->Uint16] = SIInit_u16;
        USIInit[this->Int16] = SIInit_i16;
        USIInit[this->Bool] = SIInit_b;
        inited = true;
      }
    }
  };
  extern Storage_init_interface __SII;
  ///@endcond;

}  // namespace cytnx_core

#endif  // CYTNX_CORE_STORAGE_IMPL_H_
