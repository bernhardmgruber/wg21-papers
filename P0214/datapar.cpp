namespace std {
  namespace experimental {
    template <class T, class Abi> class datapar {
    public:
      typedef T value_type;
      typedef implementation_defined reference;
      typedef mask<T, Abi> mask_type;
      typedef size_t size_type;
      typedef Abi abi_type;

      static constexpr size_type size();

      datapar() = default;

      datapar(const datapar &) = default;
      datapar(datapar &&) = default;
      datapar &operator=(const datapar &) = default;
      datapar &operator=(datapar &&) = default;

      // implicit broadcast constructor
      datapar(value_type);

      // implicit type conversion constructor
      template <class U, class Abi2> datapar(datapar<U, Abi2>);

      // load constructor
      template <class U, class Flags> datapar(const U *mem, Flags);

      // scalar access:
      reference operator[](size_type);
      value_type operator[](size_type) const;

      // increment and decrement:
      datapar &operator++();
      datapar operator++(int);
      datapar &operator--();
      datapar operator--(int);

      // unary operators (for integral T)
      mask_type operator!() const;
      datapar operator~() const;

      // unary operators (for any T)
      datapar operator+() const;
      datapar operator-() const;

      // reductions
      value_type sum() const;
      value_type sum(mask_type) const;
      value_type product() const;
      value_type product(mask_type) const;
      value_type min() const;
      value_type min(mask_type) const;
      value_type max() const;
      value_type max(mask_type) const;
    };
  }
}
