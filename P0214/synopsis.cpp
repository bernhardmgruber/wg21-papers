namespace std {
  namespace experimental {
    namespace datapar_abi {
      struct scalar {};  // always present
      template <int N> struct fixed_size {};  // always present
      constexpr int max_fixed_size = implementation_defined;
      // implementation-defined tag types, e.g. sse, avx, neon, altivec, ...
      template <typename T> using compatible = implementation_defined;  // always present
      template <typename T> using native = implementation_defined;  // always present
    }

    namespace flags {
        struct element_aligned_tag {};
        struct vector_aligned_tag {};
        template <std::align_val_t> struct overaligned_tag {};
        constexpr element_aligned_tag element_aligned{};
        constexpr vector_aligned_tag vector_aligned{};
        template <std::align_val_t N> constexpr overaligned_tag<N> overaligned = {};
    }

    // traits [datapar.traits]
    template <class T> struct is_datapar;
    template <class T> constexpr bool is_datapar_v = is_datapar<T>::value;

    template <class T> struct is_mask;
    template <class T> constexpr bool is_mask_v = is_mask<T>::value;

    template <class T, size_t N> struct abi_for_size { using type = implementation_defined; };
    template <class T, size_t N> using abi_for_size_t = typename abi_for_size<T, N>::type;

    template <class T, class Abi = datapar_abi::compatible<T>>
    struct datapar_size : public integral_constant<size_t, implementation_defined> {};
    template <class T, class Abi = datapar_abi::compatible<T>>
    constexpr size_t datapar_size_v = datapar_size<T, Abi>::value;

    template <class T, class U = typename T::value_type>
    constexpr size_t memory_alignment = implementation_defined;

    // class template datapar [datapar]
    template <class T, class Abi = datapar_abi::compatible<T>> class datapar;
    template <class T> using native_datapar = datapar<T, datapar_abi::native<T>>;
    template <class T, int N>
    using fixed_size_datapar = datapar<T, datapar_abi::fixed_size<N>>;

    // class template mask [mask]
    template <class T, class Abi = datapar_abi::compatible<T>> class mask;
    template <class T> using native_mask = mask<T, datapar_abi::native<T>>;
    template <class T, int N>
    using fixed_size_mask = mask<T, datapar_abi::fixed_size<N>>;

    // casts [datapar.casts]
    template <class T, class U, class A>
    datapar<T, /*see below*/> static_datapar_cast(const datapar<U, A> &) noexcept;

    template <class T, class A>
    datapar<T, datapar_abi::fixed_size<datapar_size_v<T, A>>> to_fixed_size(
        const datapar<T, A> &) noexcept;
    template <class T, class A>
    mask<T, datapar_abi::fixed_size<datapar_size_v<T, A>>> to_fixed_size(
        const mask<T, A> &) noexcept;
    template <class T, size_t N>
    datapar<T, datapar_abi::native<T>> to_native(
        const datapar<T, datapar_abi::fixed_size<N>> &) noexcept;
    template <class T, size_t N>
    mask<T, datapar_abi::native<T>> to_native(
        const mask<T, datapar_abi::fixed_size<N>> &) noexcept;
    template <class T, size_t N>
    datapar<T, datapar_abi::compatible<T>> to_compatible(
        const datapar<T, datapar_abi::fixed_size<N>> &) noexcept;
    template <class T, size_t N>
    mask<T, datapar_abi::compatible<T>> to_compatible(
        const mask<T, datapar_abi::fixed_size<N>> &) noexcept;

    template <class T, class U, class... Us>
    conditional_t<(T::size() == (U::size() + Us::size()...)), T,
                  array<T, (U::size() + Us::size()...) / T::size()>>
        datapar_cast(U, Us...) noexcept;

    // reductions [mask.reductions]
    template <class T, class Abi> bool  all_of(mask<T, Abi>) noexcept;
    template <class T, class Abi> bool  any_of(mask<T, Abi>) noexcept;
    template <class T, class Abi> bool none_of(mask<T, Abi>) noexcept;
    template <class T, class Abi> bool some_of(mask<T, Abi>) noexcept;
    template <class T, class Abi> int popcount(mask<T, Abi>) noexcept;
    template <class T, class Abi> int find_first_set(mask<T, Abi>) noexcept;
    template <class T, class Abi> int find_last_set(mask<T, Abi>) noexcept;

    // masked assignment [mask.where]
    template <class M, class T> class where_expression {
    public:
      const M &mask;                              // exposition only
      T &data;                                    // exposition only
      where_expression(const M &, T &) noexcept;  // exposition only

      where_expression(const where_expression &) = delete;
      where_expression &operator=(const where_expression &) = delete;

      template <class U> void operator=(U &&x);
      template <class U> void operator+=(U &&x);
      template <class U> void operator-=(U &&x);
      template <class U> void operator*=(U &&x);
      template <class U> void operator/=(U &&x);
      template <class U> void operator%=(U &&x);
      template <class U> void operator&=(U &&x);
      template <class U> void operator|=(U &&x);
      template <class U> void operator^=(U &&x);
      template <class U> void operator<<=(U &&x);
      template <class U> void operator>>=(U &&x);
      void operator++() noexcept;
      void operator++(int) noexcept;
      void operator--() noexcept;
      void operator--(int) noexcept;
      remove_const_t<T> operator-() const noexcept;

      template <class U, class Flags> void memload(const U *mem, Flags) noexcept;
      template <class U, class Flags> void memstore(U *mem, Flags) const noexcept;
    };

    template <class T, class A>
    where_expression<mask<T, A>, datapar<T, A>> where(
        const typename datapar<T, A>::mask_type &, datapar<T, A> &) noexcept;
    template <class T, class A>
    const where_expression<mask<T, A>, const datapar<T, A>> where(
        const typename datapar<T, A>::mask_type &, const datapar<T, A> &) noexcept;

    template <class T, class A>
    where_expression<mask<T, A>, mask<T, A>> where(const remove_const_t<mask<T, A>> &,
                                                   mask<T, A> &) noexcept;
    template <class T, class A>
    const where_expression<mask<T, A>, const mask<T, A>> where(
        const remove_const_t<mask<T, A>> &, const mask<T, A> &) noexcept;

    template <class T> where_expression<bool, T> where(bool k, T &d) noexcept;

    // reductions [datapar.reductions]
    template <class BinaryOperation = std::plus<>, class T, class Abi>
    T reduce(const datapar<T, Abi> &, BinaryOperation = BinaryOperation()) noexcept;
    template <class BinaryOperation = std::plus<>, class M, class V>
    typename V::value_type reduce(
        const where_expression<M, V> &x,
        typename V::value_type neutral_element = default_neutral_element,
        BinaryOperation binary_op = BinaryOperation()) noexcept;

    template <class T, class A> T hmin(const datapar<T, A> &) noexcept;
    template <class M, class V> T hmin(const where_expression<M, V> &) noexcept;
    template <class T, class A> T hmax(const datapar<T, A> &) noexcept;
    template <class M, class V> T hmax(const where_expression<M, V> &) noexcept;

    // algorithms [datapar.alg]
    template <class T, class A>
    datapar<T, A> min(const datapar<T, A> &, const datapar<T, A> &) noexcept;
    template <class T, class A>
    datapar<T, A> max(const datapar<T, A> &, const datapar<T, A> &) noexcept;
    template <class T, class A>
    std::pair<datapar<T, A>, datapar<T, A>> minmax(const datapar<T, A> &,
                                                   const datapar<T, A> &) noexcept;
    template <class T, class A>
    datapar<T, A> clamp(const datapar<T, A> &v, const datapar<T, A> &lo,
                        const datapar<T, A> &hi) noexcept;
  }
}
