#ifndef CYTNX_CORE_STORAGE_H_
#define CYTNX_CORE_STORAGE_H_

#include <cstdlib>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>
#include <memory>

#include <cytnx_core/storage_impl/Storage_impl.hpp>

namespace cytnx_core {

  ///@brief an memeory storage with multi-type/multi-device support
  class Storage {
   private:
    // Interface:
    // Storage_init_interface __SII;

   public:
    ///@cond
    std::shared_ptr<Storage_base> _impl;
    ///@endcond

    /**
    @brief initialize a Storage
    @param[in] size the number of elements for the Storage
    @param[in] dtype the dtype of the Storage instance. This can be any of type defined in
    cytnx::Type
    @param[in] device the device of the Storage instance. This can be cytnx::Device.cpu or
    cytnx::Device.cuda+<gpuid> (see cytnx::Device for more details)

    ## Example:
    ### c++ API:
    \include example/Storage/Init.cpp
    #### output>
    \verbinclude example/Storage/Init.cpp.out
    ### python API:
    \include example/Storage/Init.py
    #### output>
    \verbinclude example/Storage/Init.py.out
    */
    void Init(const unsigned long long &size, const unsigned int &dtype = Type.Double,
              int device = -1, const bool &init_zero = true) {
      cytnx_error_msg(dtype >= N_Type, "%s", "[ERROR] invalid argument: dtype");
      this->_impl = __SII.USIInit[dtype]();
      this->_impl->Init(size, device, init_zero);
    }
    // void _Init_byptr(void *rawptr, const unsigned long long &len_in, const unsigned int &dtype =
    // Type.Double, const int &device = -1,
    //                              const bool &iscap = false, const unsigned long long &cap_in =
    //                              0){
    //   cytnx_error_msg(dtype >= N_Type, "%s", "[ERROR] invalid argument: dtype");
    //   this->_impl = __SII.USIInit[dtype]();
    //   this->_impl->_Init_byptr(rawptr, len_in, device, iscap, cap_in);
    // }

    /**
     * @brief The constructor of Storage class. It will call the function
     * @ref Init(const unsigned long long &size, const unsigned int &dtype,
     *           int device, const bool &init_zero) "Init"
     * to initialize the Storage instance.
     * @see Init(const unsigned long long &size, const unsigned int &dtype, int device, const bool
     * &init_zero)
     */
    Storage(const unsigned long long &size, const unsigned int &dtype = Type.Double,
            int device = -1, const bool &init_zero = true)
        : _impl(new Storage_base()) {
      Init(size, dtype, device, init_zero);
    }
    // Storage(void *rawptr, const unsigned long long &len_in, const unsigned int &dtype =
    // Type.Double, const int &device = -1,
    //       const bool &iscap = false, const unsigned long long &cap_in = 0)
    //       : _impl(new Storage_base()){
    //   _Init_byptr(rawptr,len_in,dtype,device,iscap,cap_in);
    // }

    /**
     * @brief The default constructor of Storage class. It will create an empty Storage instance.
     */
    Storage() : _impl(new Storage_base()) {};
    ///@cond
    Storage(std::shared_ptr<Storage_base> in_impl) { this->_impl = in_impl; }
    Storage(const Storage &rhs) { this->_impl = rhs._impl; }

    template <class Tp>
    Storage(const std::vector<Tp> &rhs) {
      this->_from_vector(rhs, -1);
    }
    template <class Tp>
    Storage(const std::initializer_list<Tp> &rhs) {
      this->_from_vector(std::vector<Tp>(rhs), -1);
    }

    Storage &operator=(const Storage &rhs) {
      this->_impl = rhs._impl;
      return *this;
    }

    ///@endcond

    /// @cond
    void _Save(std::fstream &f) const;
    void _Load(std::fstream &f);
    void _Loadbinary(std::fstream &f, const unsigned int &dtype, const cytnx_uint64 &Nelem);
    void _Savebinary(std::fstream &f) const;

    /// @endcond

    /**
    @brief Save current Storage to file
    @param[in] fname file name
    @details
        Save the Storage to file with file path specify with input param \p fname with postfix
    ".cyst"
    @post The file extension will be ".cyst".
    */
    void Save(const std::string &fname) const;

    /**
     * @brief Save current Storage to file, same as \ref Save(const std::string &fname)
     */
    void Save(const char *fname) const;
    /**
     * @brief Save current Storage to a binary file, which only contains the raw data.
     * @see Fromfile(const std::string &fname, const unsigned int &dtype, const cytnx_int64 &count)
     */
    void Tofile(const std::string &fname) const;
    /// @see Tofile(const std::string &fname) const
    void Tofile(const char *fname) const;
    /// @see Tofile(const std::string &fname) const
    void Tofile(std::fstream &f) const;

    /**
    @brief Load current Storage from file
    @param[in] fname file name
    @details
        load the Storage from file with file path specify with input param 'fname'.
    @pre The file must be a Storage object, which is saved by the function
        Save(const std::string &fname) const.
    */
    static Storage Load(const std::string &fname);

    /**
     * @brief Load current Storage from file, same as \ref Load(const std::string &fname)
     */
    static Storage Load(const char *fname);
    /**
     * @brief Load the binary file, which only contains the raw data, to current Storage.
     * @details This function will load the binary file, which only contains the raw data,
     *     to current Storage with specified dtype and number of elements.
     * @param[in] fname file name
     * @param[in] dtype the data type of the binary file. See cytnx::Type.
     * @param[in] Nelem the number of elements you want to load from the binary file. If
     *   \p Nelem is -1, then it will load all the elements in the binary file.
     * @pre
     *  1. The @p dtype cannot be Type.Void.
     *  2. The @p dtype must be the same as the data type of the binary file.
     *  3. The @p Nelem cannot be 0.
     *  4. The @p Nelem cannot be larger than the number of elements in the binary file.
     *  5. The file name @p fname must be valid.
     *
     * @see Tofile(const std::string &fname) const
     */
    static Storage Fromfile(const std::string &fname, const unsigned int &dtype,
                            const cytnx_int64 &count = -1);

    /**
     * @see Fromfile(const std::string &fname, const unsigned int &dtype, const cytnx_int64 &count =
     * -1)
     */
    static Storage Fromfile(const char *fname, const unsigned int &dtype,
                            const cytnx_int64 &count = -1);

    /**
    @brief cast the type of current Storage
    @details
        1. if the new_type is the same as the dtype of current Storage, then return self;
           otherwise, return a new instance that has the same content as current Storage with
           dtype= \p new_type .
        2. the return Stoarge will be on the same device as the current Storage.
    @param[in] new_type the new type of the Storage instance. This can be any of type defined in
    cytnx::Type.
    @attention
        This function cannot convert the complex type to real one. Please use real() or
          imag() to get the real or imaginary part of a complex Storage.


    ## Example:
    ### c++ API:
    \include example/Storage/astype.cpp
    #### output>
    \verbinclude example/Storage/astype.cpp.out
    ### python API:
    \include example/Storage/astype.py
    #### output>
    \verbinclude example/Storage/astype.py.out
    */
    Storage astype(const unsigned int &new_type) const { return this->_impl->astype(new_type); }

    /**
    @brief the dtype-id of current Storage, see cytnx::Type for more details.
    @return [cytnx_uint64] the dtype-id.
    */
    const unsigned int &dtype() const { return this->_impl->dtype; }

    /**
    @brief the dtype (std::string) of current Storage, see cytnx::Type for more details.
    @return [std::string] dtype name
    */
    const std::string dtype_str() const {
      std::string out = this->_impl->dtype_str();
      return out;
    }
    /**
    @brief the device-id of current Storage, see cytnx::Device for more details.
    @return [cytnx_int64] the device-id.
    */
    const int &device() const { return this->_impl->device; }

    /**
    @brief the device (std::string) of current Storage, see cytnx::Device for more details.
    @return [std::string] device name
    */
    const std::string device_str() const {
      std::string out = this->_impl->device_str();
      return out;
    }

    /**
    @brief append a value
    @param[in] val the value to append. it can be any type defined in cytnx::Type
    @note cannot append a complex value into a real Storage.
    */
    template <class T>
    void append(const T &val) {
      return this->_impl->append(val);
    }

    ///@cond
    template <class T>  // this is c++ only
    T &at(const cytnx_uint64 &idx) const {
      return this->_impl->at<T>(idx);
    }

    // const Scalar::Sproxy at(const cytnx_uint64 &idx) const {
    //   Scalar::Sproxy out(this->_impl, idx);
    //   return out;
    // }
    // Scalar::Sproxy at(const cytnx_uint64 &idx) {
    //   Scalar::Sproxy out(this->_impl, idx);
    //   return out;
    // }

    template <class T>  // this is c++ only
    T &back() const {
      return this->_impl->back<T>();
    }

    // const Scalar::Sproxy back() const {
    //   Scalar::Sproxy out(this->_impl, this->size() - 1);
    //   return out;
    // }
    // Scalar::Sproxy back() {
    //   Scalar::Sproxy out(this->_impl, this->size() - 1);
    //   return out;
    // }

    template <class T>  // this is c++ only
    T *data() const {
      return this->_impl->data<T>();
    }

    void *data() const { return this->_impl->data(); }
    ///@endcond

    /**
    @brief resize the current Storage.
    @param[in] newsize.
    */
    void resize(const cytnx_uint64 &newsize) { this->_impl->resize(newsize); }

    /**
    @brief Move the current Storage to different deivce.
    @param[in] device the device-id. It can be any device defined in cytnx::Device.
    @see Storage::to()
    */
    void to_(const int &device) { this->_impl->to_(device); }

    /**
    @brief move a new Storage with same content as current Storage on different deivce.
    @param[in] device the device-id. It can be any device defined in cytnx::Device
    @note
        if the \p device is the same as the current Storage's device, return self.
    @see Storage::to_()
    */
    Storage to(const int &device) { return Storage(this->_impl->to(device)); }

    /**
    @brief return a deep copy of the current storage.
    @return
        [Storage]

    ## Example:
    ### c++ API:
    \include example/Storage/clone.cpp
    #### output>
    \verbinclude example/Storage/clone.cpp.out
    ### python API:
    \include example/Storage/clone.py
    #### output>
    \verbinclude example/Storage/clone.py.out
    */
    Storage clone() const { return Storage(this->_impl->clone()); }

    /**
    @brief the size ( no. of elements ) in the Storage
    @return [cytnx_uint64]

    */
    const unsigned long long &size() const { return this->_impl->len; }

    /**
    @brief the capacity in the Storage.
    @details the capacity is the actual allocated memory in the Storage. The behavior of
      capacity is similar to std::vector::capacity() in c++.
    @return [cytnx_uint64]

    */
    const unsigned long long &capacity() const { return this->_impl->cap; }

    /**
    @brief print the info of the Storage, including the device, dtype and size.

    */
    void print_info() const { this->_impl->print_info(); }
    /// @cond
    // this is a redundant function
    void print() const { this->_impl->print(); }
    /// @endcond

    /**
    @brief set all the elements to zero.
    @note  although it is also possible to use Storage.fill(0) to set all the elements to zero,
           using set_zeros will have significant faster performance.

    */
    void set_zeros() { this->_impl->set_zeros(); }

    /**
    @brief compare two Storage
    @details This function will compare the content between two Storage objects. It will compare the
        "value" of each element. Even the two Storage are different objects (different instance), if
        they have the same values, this function will return true.
    @param[in] Storage another Storage to compare to
    @note the == operator will compare the content between two storages. use cytnx::is() for
    checking two variables share the same instance.

    ## Example:
    ### c++ API:
    \include example/Storage/eq.cpp
    #### output>
    \verbinclude example/Storage/eq.cpp.out
    ### python API:
    \include example/Storage/eq.py
    #### output>
    \verbinclude example/Storage/eq.py.out
    */
    bool operator==(const Storage &rhs);

    /**
     * @brief The not-equal operator for Storage.
     */
    bool operator!=(const Storage &rhs);

    /**
    @brief set all the elements to the assigned value val
    @param[in] val the value to set on all the elements. it can be any type defined in cytnx::Type
    @note cannot assign a complex value into a real Storage.
    */
    template <class T>
    void fill(const T &val) {
      this->_impl->fill(val);
    }

    /**
    @brief renew/create a Storage using c++ vector.
    @param[in] vin the C++ vector with supported types.
    @note This function is C++ only
    */
    template <class T>
    static Storage from_vector(const std::vector<T> &vin, const int device = -1) {
      Storage out;
      out._from_vector(vin, device);
      return out;
    }

    /*
        @brief convert a Storage to C++ vector.

        [Note]
            This function is C++ only
    */
    /*
    template <class T>
    std::vector<T> vector() {
      T tmp;
      cytnx_error_msg(Type.cy_typeid(tmp) != this->dtype(),
                      "[ERROR] the dtype of current Storage does not match assigned vector type.%s",
                      "\n");

      std::vector<T> out(this->size());
      Storage S;
      if (this->device() != Device.cpu) {
        S = this->to(Device.cpu);
        memcpy(&out[0], S.data(), sizeof(T) * this->size());
      } else {
        memcpy(&out[0], this->data(), sizeof(T) * this->size());
      }

      return out;
    }
    */

    /**
    @brief renew/create a c++ std::vector using current Storage.
    @note This function is C++ only
    */
    template <class T>
    std::vector<T> vector();

    /// @cond

    template <class T>
    void _from_vector(const std::vector<T> &vin, const int device = -1) {
      // auto dispatch:
      // check:
      cytnx_error_msg(1, "[FATAL] ERROR unsupport type%s", "\n");
      // this->_impl->Init(vin.size(),device);
      // memcpy(this->_impl->Mem,&vin[0],sizeof(T)*vin.size());
    }

    void _from_vector(const std::vector<cytnx_complex128> &vin, const int device = -1) {
      this->_impl = __SII.USIInit[Type.ComplexDouble]();
      this->_impl->Init(vin.size(), device);
      memcpy(this->_impl->Mem, &vin[0], sizeof(cytnx_complex128) * vin.size());
    }
    void _from_vector(const std::vector<cytnx_complex64> &vin, const int device = -1) {
      this->_impl = __SII.USIInit[Type.ComplexFloat]();
      this->_impl->Init(vin.size(), device);
      memcpy(this->_impl->Mem, &vin[0], sizeof(cytnx_complex64) * vin.size());
    }
    void _from_vector(const std::vector<cytnx_double> &vin, const int device = -1) {
      this->_impl = __SII.USIInit[Type.Double]();
      this->_impl->Init(vin.size(), device);
      memcpy(this->_impl->Mem, &vin[0], sizeof(cytnx_double) * vin.size());
    }
    void _from_vector(const std::vector<cytnx_float> &vin, const int device = -1) {
      this->_impl = __SII.USIInit[Type.Float]();
      this->_impl->Init(vin.size(), device);
      memcpy(this->_impl->Mem, &vin[0], sizeof(cytnx_float) * vin.size());
    }
    void _from_vector(const std::vector<cytnx_uint64> &vin, const int device = -1) {
      this->_impl = __SII.USIInit[Type.Uint64]();
      this->_impl->Init(vin.size(), device);
      memcpy(this->_impl->Mem, &vin[0], sizeof(cytnx_uint64) * vin.size());
    }
    void _from_vector(const std::vector<cytnx_int64> &vin, const int device = -1) {
      this->_impl = __SII.USIInit[Type.Int64]();
      this->_impl->Init(vin.size(), device);
      memcpy(this->_impl->Mem, &vin[0], sizeof(cytnx_int64) * vin.size());
    }
    void _from_vector(const std::vector<cytnx_uint32> &vin, const int device = -1) {
      this->_impl = __SII.USIInit[Type.Uint32]();
      this->_impl->Init(vin.size(), device);
      memcpy(this->_impl->Mem, &vin[0], sizeof(cytnx_uint32) * vin.size());
    }
    void _from_vector(const std::vector<cytnx_int32> &vin, const int device = -1) {
      this->_impl = __SII.USIInit[Type.Int32]();
      this->_impl->Init(vin.size(), device);
      memcpy(this->_impl->Mem, &vin[0], sizeof(cytnx_int32) * vin.size());
    }
    void _from_vector(const std::vector<cytnx_uint16> &vin, const int device = -1) {
      this->_impl = __SII.USIInit[Type.Uint16]();
      this->_impl->Init(vin.size(), device);
      memcpy(this->_impl->Mem, &vin[0], sizeof(cytnx_uint16) * vin.size());
    }
    void _from_vector(const std::vector<cytnx_int16> &vin, const int device = -1) {
      this->_impl = __SII.USIInit[Type.Int16]();
      this->_impl->Init(vin.size(), device);
      memcpy(this->_impl->Mem, &vin[0], sizeof(cytnx_int16) * vin.size());
    }
    void _from_vector(const std::vector<cytnx_bool> &vin, const int device = -1) {
      this->_impl = __SII.USIInit[Type.Bool]();
      this->_impl->Init(vin.size(), device);
      this->_impl->_cpy_bool(this->_impl->Mem, vin);
      // memcpy(this->_impl->Mem,vin.data(),sizeof(cytnx_bool)*vin.size());
    }
    /// @endcond

    /**
    @brief Get the real part form a Complex type Storage
    @note Cannot be called from a real type Storage.

    ## Example:
    ### c++ API:
    \include example/Storage/real.cpp
    #### output>
    \verbinclude example/Storage/real.cpp.out
    ### python API:
    \include example/Storage/real.py
    #### output>
    \verbinclude example/Storage/real.py.out
    */
    Storage real() const { return Storage(this->_impl->real()); };

    /**
    @brief Get the imaginary part form a Complex type Storage
    @note Cannot be called from a real type Storage.

    ## Example:
    ### c++ API:
    \include example/Storage/imag.cpp
    #### output>
    \verbinclude example/Storage/imag.cpp.out
    ### python API:
    \include example/Storage/imag.py
    #### output>
    \verbinclude example/Storage/imag.py.out
    */
    Storage imag() const { return Storage(this->_impl->imag()); };

    /**
     * @brief Get the element at the given index.
     * @param[in] idx The index of the element.
     * @return The element at the given index.
     */
    // Scalar get_item(const cytnx_uint64 &idx) const { return this->_impl->get_item(idx); };

    /**
     * @brief Set the element at the given index.
     * @param[in] idx The index of the element.
     * @param[in] elem The element to be set.
     */
    template <class T>
    void set_item(const cytnx_uint64 &idx, const T &elem) {
      this->_impl->set_item(idx, elem);
    };

    /**
     * @brief The access operator for the Storage.
     * @param[in] idx The index of the element.
     */
    // Scalar::Sproxy operator()(const cytnx_uint64 &idx);

    // /**
    //  * @brief Check if two Storage are equivalent in element.
    //  * @param[in] rhs The Storage to compare.
    //  * @note Two Storage are equivalent if they have the same type, and elements.
    //  * @return If two Storage are equivalent.
    //  */
    // bool approx_eq(const Storage &rhs, const cytnx_double tol = 0) const {
    //   return this->_impl->approx_eq(rhs._impl, tol);
    // };
  };

  ///@cond
  std::ostream &operator<<(std::ostream &os, const Storage &in);
  ///@endcond

}  // namespace cytnx_core

#endif  // CYTNX_CORE_STORAGE_H_
