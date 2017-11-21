namespace std::experimental {
  inline namespace parallelism_v2 {
    template <class M, class T> class const_where_expression {
      const M& mask;  // exposition only
      T& data;        // exposition only

    public:
      const_where_expression(const const_where_expression&) = delete;
      const_where_expression& operator=(const const_where_expression&) = delete;

      remove_const_t<T> operator-() const &&;

      template <class U, class Flags>
      [[nodiscard]] V copy_from(const U* mem, Flags f) const &&;
      // rename the above to disambiguate with where_expression::copy_from:
      // - merge_from
      // - merge_with_copy_from
      // - ?
      template <class U, class Flags> void copy_to(U* mem, Flags f) const &&;
    };

    template <class M, class T>
    class where_expression : public const_where_expression<M, T> {
    public:
      where_expression(const where_expression&) = delete;
      where_expression& operator=(const where_expression&) = delete;

      template <class U> void operator=(U&& x);
      template <class U> void operator+=(U&& x);
      template <class U> void operator-=(U&& x);
      template <class U> void operator*=(U&& x);
      template <class U> void operator/=(U&& x);
      template <class U> void operator%=(U&& x);
      template <class U> void operator&=(U&& x);
      template <class U> void operator|=(U&& x);
      template <class U> void operator^=(U&& x);
      template <class U> void operator<<=(U&& x);
      template <class U> void operator>>=(U&& x);
      void operator++();
      void operator++(int);
      void operator--();
      void operator--(int);

      template <class U, class Flags> void copy_from(const U* mem, Flags);
    };
  }
}
