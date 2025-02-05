// RUN: %check_clang_tidy %s cppcoreguidelines-no-temporary-expr-stmt %t

struct Trivial {};
void test0() {
  Trivial{}; // CHECK-MESSAGES: :[[@LINE]]:3: warning: temporary destroyed at the end of full-expression; did you mean to create a variable? [cppcoreguidelines-no-temporary-expr-stmt]
  // CHECK-FIXES: Trivial {{[a-zA-Z_][a-zA-Z0-9_]*}}{};
  Trivial(); // CHECK-MESSAGES: :[[@LINE]]:3: warning: temporary destroyed at the end of full-expression; did you mean to create a variable? [cppcoreguidelines-no-temporary-expr-stmt]
  // CHECK-FIXES: auto {{[a-zA-Z_][a-zA-Z0-9_]*}} = Trivial();
}
struct NonTrivialCtor {
  NonTrivialCtor();
  NonTrivialCtor(int);
};
void test1() {
  NonTrivialCtor{}; // CHECK-MESSAGES: :[[@LINE]]:3: warning: temporary destroyed at the end of full-expression; did you mean to create a variable? [cppcoreguidelines-no-temporary-expr-stmt]
  // CHECK-FIXES: NonTrivialCtor {{[a-zA-Z_][a-zA-Z0-9_]*}}{};
  NonTrivialCtor(); // CHECK-MESSAGES: :[[@LINE]]:3: warning: temporary destroyed at the end of full-expression; did you mean to create a variable? [cppcoreguidelines-no-temporary-expr-stmt]
  // CHECK-FIXES: auto {{[a-zA-Z_][a-zA-Z0-9_]*}} = NonTrivialCtor();
  NonTrivialCtor{0}; // CHECK-MESSAGES: :[[@LINE]]:3: warning: temporary destroyed at the end of full-expression; did you mean to create a variable? [cppcoreguidelines-no-temporary-expr-stmt]
  // CHECK-FIXES: NonTrivialCtor {{[a-zA-Z_][a-zA-Z0-9_]*}}{0};
  NonTrivialCtor(0); // CHECK-MESSAGES: :[[@LINE]]:3: warning: temporary destroyed at the end of full-expression; did you mean to create a variable? [cppcoreguidelines-no-temporary-expr-stmt]
  // CHECK-FIXES: NonTrivialCtor {{[a-zA-Z_][a-zA-Z0-9_]*}}(0);
}
struct NonTrivialDtor { ~NonTrivialDtor(); };
void test2() {
  NonTrivialDtor{}; // CHECK-MESSAGES: :[[@LINE]]:3: warning: temporary destroyed at the end of full-expression; did you mean to create a variable? [cppcoreguidelines-no-temporary-expr-stmt]
  // CHECK-FIXES: NonTrivialDtor {{[a-zA-Z_][a-zA-Z0-9_]*}}{};
  NonTrivialDtor(); // CHECK-MESSAGES: :[[@LINE]]:3: warning: temporary destroyed at the end of full-expression; did you mean to create a variable? [cppcoreguidelines-no-temporary-expr-stmt]
  // CHECK-FIXES: auto {{[a-zA-Z_][a-zA-Z0-9_]*}} = NonTrivialDtor();
}
struct NontrivialCtorDtor { 
  NontrivialCtorDtor();
  NontrivialCtorDtor(int);
  ~NontrivialCtorDtor();
};
NontrivialCtorDtor f();
void test3() {
  NontrivialCtorDtor{}; // CHECK-MESSAGES: :[[@LINE]]:3: warning: temporary destroyed at the end of full-expression; did you mean to create a variable? [cppcoreguidelines-no-temporary-expr-stmt]
  // CHECK-FIXES: NontrivialCtorDtor {{[a-zA-Z_][a-zA-Z0-9_]*}}{};
  NontrivialCtorDtor(); // CHECK-MESSAGES: :[[@LINE]]:3: warning: temporary destroyed at the end of full-expression; did you mean to create a variable? [cppcoreguidelines-no-temporary-expr-stmt]
  // CHECK-FIXES: auto {{[a-zA-Z_][a-zA-Z0-9_]*}} = NontrivialCtorDtor();
  NontrivialCtorDtor{0}; // CHECK-MESSAGES: :[[@LINE]]:3: warning: temporary destroyed at the end of full-expression; did you mean to create a variable? [cppcoreguidelines-no-temporary-expr-stmt]
  // CHECK-FIXES: NontrivialCtorDtor {{[a-zA-Z_][a-zA-Z0-9_]*}}{0};
  NontrivialCtorDtor(0); // CHECK-MESSAGES: :[[@LINE]]:3: warning: temporary destroyed at the end of full-expression; did you mean to create a variable? [cppcoreguidelines-no-temporary-expr-stmt]
  // CHECK-FIXES: NontrivialCtorDtor {{[a-zA-Z_][a-zA-Z0-9_]*}}(0);
}

struct Test {
  Test() {}
  Test(int) {}
  Test(int, int) {}
  operator bool() const { return true; }
  ~Test() {}
};

template <class T>
void test4() {
  T(0, 0); // CHECK-MESSAGES: :[[@LINE]]:3: warning: temporary destroyed at the end of full-expression; did you mean to create a variable? [cppcoreguidelines-no-temporary-expr-stmt]
  // CHECK-FIXES: T {{[a-zA-Z_][a-zA-Z0-9_]*}}(0, 0);
  T{0, 0}; // CHECK-MESSAGES: :[[@LINE]]:3: warning: temporary destroyed at the end of full-expression; did you mean to create a variable? [cppcoreguidelines-no-temporary-expr-stmt]
  // CHECK-FIXES: T {{[a-zA-Z_][a-zA-Z0-9_]*}}{0, 0};
  T{T{}}; // CHECK-MESSAGES: :[[@LINE]]:3: warning: temporary destroyed at the end of full-expression; did you mean to create a variable? [cppcoreguidelines-no-temporary-expr-stmt]
  // CHECK-FIXES: T {{[a-zA-Z_][a-zA-Z0-9_]*}}{T{}};
  T(T{}); // CHECK-MESSAGES: :[[@LINE]]:3: warning: temporary destroyed at the end of full-expression; did you mean to create a variable? [cppcoreguidelines-no-temporary-expr-stmt]
  // CHECK-FIXES: T {{[a-zA-Z_][a-zA-Z0-9_]*}}(T{});
  T{T()}; // CHECK-MESSAGES: :[[@LINE]]:3: warning: temporary destroyed at the end of full-expression; did you mean to create a variable? [cppcoreguidelines-no-temporary-expr-stmt]
  // CHECK-FIXES: T {{[a-zA-Z_][a-zA-Z0-9_]*}}{T()};
  T{T{T()}}; // CHECK-MESSAGES: :[[@LINE]]:3: warning: temporary destroyed at the end of full-expression; did you mean to create a variable? [cppcoreguidelines-no-temporary-expr-stmt]
  // CHECK-FIXES: T {{[a-zA-Z_][a-zA-Z0-9_]*}}{T{T()}};
  T(T(T{})); // CHECK-MESSAGES: :[[@LINE]]:3: warning: temporary destroyed at the end of full-expression; did you mean to create a variable? [cppcoreguidelines-no-temporary-expr-stmt]
  // CHECK-FIXES: T {{[a-zA-Z_][a-zA-Z0-9_]*}}(T(T{}));
  {
    T{0}; // CHECK-MESSAGES: :[[@LINE]]:5: warning: temporary destroyed at the end of full-expression; did you mean to create a variable? [cppcoreguidelines-no-temporary-expr-stmt]
    // CHECK-FIXES: T {{[a-zA-Z_][a-zA-Z0-9_]*}}{0};
  }
  [] {
    T{0}; // CHECK-MESSAGES: :[[@LINE]]:5: warning: temporary destroyed at the end of full-expression; did you mean to create a variable? [cppcoreguidelines-no-temporary-expr-stmt]
    // CHECK-FIXES: T {{[a-zA-Z_][a-zA-Z0-9_]*}}{0};
  };

  T a(0);
  T(static_cast<T &&>(a)); // CHECK-MESSAGES: :[[@LINE]]:3: warning: temporary destroyed at the end of full-expression; did you mean to create a variable? [cppcoreguidelines-no-temporary-expr-stmt]
  // CHECK-FIXES: T {{[a-zA-Z_][a-zA-Z0-9_]*}}(static_cast<T &&>(a));

  decltype(T{}) b;
  const T &c = T();
  sizeof T{};
  (T{});
  (void)T{};
  T{}, 0;
  (T{}, 0);
  (0, T{});
}

void test() {
  test4<Test>();
}

template <class T>
void b(const T &) {}

Test c();

void d() {
  b(Test{});
  c();
  if (Test{}) {
    Test{}; // CHECK-MESSAGES: :[[@LINE]]:5: warning: temporary destroyed at the end of full-expression; did you mean to create a variable? [cppcoreguidelines-no-temporary-expr-stmt]
    // CHECK-FIXES: Test {{[a-zA-Z_][a-zA-Z0-9_]*}}{};
  }
  for (const Test &f2 = Test{}; !f2; ) {
    Test{}; // CHECK-MESSAGES: :[[@LINE]]:5: warning: temporary destroyed at the end of full-expression; did you mean to create a variable? [cppcoreguidelines-no-temporary-expr-stmt]
    // CHECK-FIXES: Test {{[a-zA-Z_][a-zA-Z0-9_]*}}{};
  }
  while (!Test{})
    Test{};
}
