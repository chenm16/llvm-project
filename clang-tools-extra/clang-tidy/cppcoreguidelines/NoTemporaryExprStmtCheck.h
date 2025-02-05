//===--- NoTemporaryExprStmtCheck.h - clang-tidy ----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_CPPCOREGUIDELINES_NOTEMPORARYEXPRSTMTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_CPPCOREGUIDELINES_NOTEMPORARYEXPRSTMTCHECK_H

#include "../ClangTidyCheck.h"

namespace clang::tidy::cppcoreguidelines {

/// ES.84: Don’t try to declare a local variable with no name.
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/cppcoreguidelines/no-temporary-expr-stmt.html
class NoTemporaryExprStmtCheck : public ClangTidyCheck {
public:
  NoTemporaryExprStmtCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

private:
  DiagnosticBuilder diagTempOrFnCast(const ast_matchers::MatchFinder::MatchResult &Result, const Expr *TempOrFnCast);

  unsigned newVarCounter = 0;
};

} // namespace clang::tidy::cppcoreguidelines

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_CPPCOREGUIDELINES_NOTEMPORARYEXPRSTMTCHECK_H
