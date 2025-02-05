//===--- NoTemporaryExprStmtCheck.cpp - clang-tidy -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "NoTemporaryExprStmtCheck.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <llvm/ADT/Twine.h>

using namespace clang::ast_matchers;

namespace clang::tidy::cppcoreguidelines {

void NoTemporaryExprStmtCheck::registerMatchers(MatchFinder *Finder) {
  auto isBlockFullExpr = hasParent(stmt(anyOf(
    exprWithCleanups(hasParent(compoundStmt())),
    compoundStmt()
  )));
  Finder->addMatcher(
    expr(anyOf(
      cxxTemporaryObjectExpr(anyOf(
        hasParent(cxxBindTemporaryExpr(isBlockFullExpr)),
        isBlockFullExpr
      )).bind("temporary_obj"),
      cxxFunctionalCastExpr(isBlockFullExpr).bind("functional_cast")
    )), this
  );
}

DiagnosticBuilder NoTemporaryExprStmtCheck::diagTempOrFnCast(const MatchFinder::MatchResult &Result, const Expr *TempOrFnCast) {
  auto Diag = diag(TempOrFnCast->getExprLoc(), "temporary destroyed at the end of full-expression; did you mean to create a variable?");
  Diag.AddSourceRange(Result.SourceManager->getExpansionRange(TempOrFnCast->getSourceRange()));
  return Diag;
}

void NoTemporaryExprStmtCheck::check(const MatchFinder::MatchResult &Result) {
  if (const auto *FnCastExpr = Result.Nodes.getNodeAs<CXXFunctionalCastExpr>("functional_cast")) {
    SourceLocation insertLoc = FnCastExpr->getLParenLoc();
    // handle braced initializer
    if (insertLoc.isInvalid())
      insertLoc = FnCastExpr->getSubExpr()->getBeginLoc();
    diagTempOrFnCast(Result, FnCastExpr)
        << FixItHint::CreateInsertion(insertLoc,
            (llvm::Twine(' ') + "var" + llvm::Twine(newVarCounter++)).str());
    return;
  }
  const auto *TempExpr = Result.Nodes.getNodeAs<CXXTemporaryObjectExpr>("temporary_obj");
  diagTempOrFnCast(Result, TempExpr) << (
    // avoid making it a function declaration
    TempExpr->getNumArgs() == 0 && !TempExpr->isListInitialization()
        ? FixItHint::CreateInsertion(TempExpr->getBeginLoc(),
            ("auto var" + llvm::Twine(newVarCounter++) + " = ").str())
        : FixItHint::CreateInsertion(TempExpr->getParenOrBraceRange().getBegin(),
            (llvm::Twine(' ') + "var" + llvm::Twine(newVarCounter++)).str())
  );
}

} // namespace clang::tidy::cppcoreguidelines
