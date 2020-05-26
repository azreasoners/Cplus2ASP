#pragma once

#include "bcplus/elements/formulas.h"
#include "bcplus/statements/Statement.h"
#include "bcplus/statements/detail/ListStatement.h"
#include "bcplus/statements/detail/ObjectBinding.h"
#include "bcplus/statements/detail/VoidStatement.h"
#include "bcplus/statements/detail/SimpleStatement.h"
#include "bcplus/symbols/MacroSymbol.h"
#include "bcplus/symbols/ConstantSymbol.h"
#include "bcplus/symbols/SortSymbol.h"
#include "bcplus/symbols/VariableSymbol.h"

namespace bcplus {
namespace statements {


/// Macro definition
typedef detail::ListStatement<Statement::Type::MACROS, symbols::MacroSymbol> MacroDeclaration;

/// Constant definition
typedef detail::ListStatement<Statement::Type::CONSTANTS, symbols::ConstantSymbol> ConstantDeclaration;

/// Object definition
typedef detail::ListStatement<Statement::Type::OBJECTS, detail::ObjectBinding> ObjectDeclaration;

/// Sort definition
typedef detail::ListStatement<Statement::Type::SORTS, symbols::SortSymbol> SortDeclaration;

/// Variable definition
typedef detail::ListStatement<Statement::Type::VARIABLES, symbols::VariableSymbol> VariableDeclaration;

/// Show statement
typedef detail::ListStatement<Statement::Type::SHOW, elements::AtomicFormula> ShowStatement;

/// Show all statement
typedef detail::VoidStatement<Statement::Type::SHOW_ALL> ShowAllStatement;

/// Hide statement
typedef detail::ListStatement<Statement::Type::HIDE, elements::AtomicFormula> HideStatement;

/// Hide all statement
typedef detail::VoidStatement<Statement::Type::HIDE_ALL> HideAllStatement;

/// noconcurrency
typedef detail::VoidStatement<Statement::Type::NOCONCURRENCY> NCStatement;

/// strong noconcurrency
typedef detail::VoidStatement<Statement::Type::STRONG_NOCONCURRENCY> StrongNCStatement;

/// maxAFValue declaration
typedef detail::SimpleStatement<Statement::Type::MAXAFVALUE, int, int> MaxAFValueStatement;

/// maxAdditive declaration
typedef detail::SimpleStatement<Statement::Type::MAXADDITIVE, int, int> MaxAdditiveStatement;

/// Include statements
typedef detail::ListStatement<Statement::Type::INCLUDE, const ReferencedString> IncludeStatement;

}}

