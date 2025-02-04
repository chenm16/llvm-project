//===--- AvoidMemberSymmetricOpCheck.cpp - clang-tidy ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidMemberSymmetricOpCheck.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::cppcoreguidelines {

AvoidMemberSymmetricOpCheck::AvoidMemberSymmetricOpCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context),
      CheckBinPlus(Options.get("CheckBinaryPlus", false)) {}

void AvoidMemberSymmetricOpCheck::storeOptions(ClangTidyOptions::OptionMap &Opts) {
  Options.store(Opts, "CheckBinaryPlus", CheckBinPlus);
}

void AvoidMemberSymmetricOpCheck::registerMatchers(MatchFinder *Finder) {
  // Skip checking equality operators in C++20 or later, since rewritten candidates consider swapped operands.
  if (!getLangOpts().CPlusPlus20)
    Finder->addMatcher(
      cxxMethodDecl(
        anyOf(hasName("operator=="), hasName("operator!="))
      ).bind("equality_op"), this
    );
  if (CheckBinPlus)
    Finder->addMatcher(
      cxxMethodDecl(
        hasName("operator+"),
        anyOf(
          allOf(isExplicitObjectMemberFunction(), parameterCountIs(2)),
          allOf(unless(isExplicitObjectMemberFunction()), parameterCountIs(1))
        )
      ).bind("bin_plus_op"), this
    );
}

void AvoidMemberSymmetricOpCheck::DiagnoseMemberSymmetricOp(const CXXMethodDecl *SymmetricOp) {
  diag(SymmetricOp->getLocation(), "symmetric operator %0 declared as a member function")
      << SymmetricOp;
  diag(SymmetricOp->getLocation(), "use non-member function, e.g. hidden friend, instead", DiagnosticIDs::Note);
}

void AvoidMemberSymmetricOpCheck::check(const MatchFinder::MatchResult &Result) {
  if (const auto *EqOp = Result.Nodes.getNodeAs<CXXMethodDecl>("equality_op"))
    DiagnoseMemberSymmetricOp(EqOp);
  else if (const auto *PlusOp = Result.Nodes.getNodeAs<CXXMethodDecl>("bin_plus_op"))
    DiagnoseMemberSymmetricOp(PlusOp);
}

} // namespace clang::tidy::cppcoreguidelines
