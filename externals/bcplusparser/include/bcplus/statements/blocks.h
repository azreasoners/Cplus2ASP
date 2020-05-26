#pragma once

#include "bcplus/statements/Statement.h"
#include "bcplus/parser/Token.h"
#include "bcplus/statements/detail/SimpleStatement.h"
#include "bcplus/statements/detail/ListStatement.h"

namespace bcplus {
namespace statements {

/// A statement containing code destined for F2LP
typedef detail::SimpleStatement<Statement::Type::F2LP, babb::utils::ref_ptr<const parser::Token>, parser::Token const* > F2LPBlock;

/// A statement contianing code destined for the ASP solver
typedef detail::SimpleStatement<Statement::Type::ASP, babb::utils::ref_ptr<const parser::Token>, parser::Token const* > ASPBlock;

/// A statement containing LUA code
typedef detail::SimpleStatement<Statement::Type::LUA, babb::utils::ref_ptr<const parser::Token>, parser::Token const* > LUABlock;

/// A statement containing PYTHON code
typedef detail::SimpleStatement<Statement::Type::PYTHON, babb::utils::ref_ptr<const parser::Token>, parser::Token const* > PYTHONBlock;

/// A statement containing aggregate comments
typedef detail::ListStatement<Statement::Type::COMMENTS, const parser::Token> CommentBlock;

}}
