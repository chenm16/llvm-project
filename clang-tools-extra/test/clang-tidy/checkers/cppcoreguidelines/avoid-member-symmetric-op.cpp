// RUN: %check_clang_tidy -std=c++17 -check-suffixes=PRECXX20 \
// RUN:     %s cppcoreguidelines-avoid-member-symmetric-op %t
// RUN: %check_clang_tidy -std=c++17 -check-suffixes=BINPLUS,PRECXX20 \
// RUN:     %s cppcoreguidelines-avoid-member-symmetric-op %t -- \
// RUN:     -config="{CheckOptions: { \
// RUN:         cppcoreguidelines-avoid-member-symmetric-op.CheckBinaryPlus: true \
// RUN:     }}"
// RUN: %check_clang_tidy -std=c++20 -check-suffixes=BINPLUS \
// RUN:     %s cppcoreguidelines-avoid-member-symmetric-op %t -- \
// RUN:     -config="{CheckOptions: { \
// RUN:         cppcoreguidelines-avoid-member-symmetric-op.CheckBinaryPlus: true \
// RUN:     }}"
// RUN: %check_clang_tidy -std=c++23 -check-suffixes=BINPLUS,BINPLUS23 \
// RUN:     %s cppcoreguidelines-avoid-member-symmetric-op %t -- \
// RUN:     -config="{CheckOptions: { \
// RUN:         cppcoreguidelines-avoid-member-symmetric-op.CheckBinaryPlus: true \
// RUN:     }}"

struct Test1 {
  bool operator==(Test1 const &) const;
  // CHECK-MESSAGES-PRECXX20: :[[@LINE-1]]:10: warning: symmetric operator 'operator==' declared as a member function [cppcoreguidelines-avoid-member-symmetric-op]

  bool operator!=(Test1 const &) const;
  // CHECK-MESSAGES-PRECXX20: :[[@LINE-1]]:10: warning: symmetric operator 'operator!=' declared as a member function [cppcoreguidelines-avoid-member-symmetric-op]
};

// test members of class template
template <class>
struct Test2 {
  constexpr bool operator==(Test2 const &) const { return true; }
  // CHECK-MESSAGES-PRECXX20: :[[@LINE-1]]:20: warning: symmetric operator 'operator==' declared as a member function [cppcoreguidelines-avoid-member-symmetric-op]

  Test2 operator+(Test2 const &) const;
  // CHECK-MESSAGES-BINPLUS: :[[@LINE-1]]:11: warning: symmetric operator 'operator+' declared as a member function [cppcoreguidelines-avoid-member-symmetric-op]
};

static_assert(Test2<int>{} == Test2<int>{});

#if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
struct Test3 {
  Test3 operator+(this auto &&, const Test3 &);
  // CHECK-MESSAGES-BINPLUS23: :[[@LINE-1]]:11: warning: symmetric operator 'operator+' declared as a member function [cppcoreguidelines-avoid-member-symmetric-op]

  Test3 operator+(this auto &&); // Ok, Unary +
};
#endif

class Test4 {};
inline bool operator==(const Test4 &, const Test4 &); // Ok: non-member function

struct Test5 {
  friend bool operator==(const Test5 &, const Test5 &); // Ok: non-member function, hidden friend
  Test5 operator+() const; // Ok: Unary +
  Test5 operator-(Test5 const &) const; // Ok
};
