#pragma once

#include "bcplus/elements/formulas.h"
#include "bcplus/elements/terms.h"
#include "bcplus/parser/Number.h"
#include "bcplus/statements/Statement.h"

#include "bcplus/statements/detail/BasicLawForm.h"
#include "bcplus/statements/detail/ConstraintLawForm.h"
#include "bcplus/statements/detail/DynamicLawForm.h"
#include "bcplus/statements/detail/DynamicConstraintLawForm.h"
#include "bcplus/statements/detail/DynamicValueLawForm.h"
#include "bcplus/statements/detail/SimpleLawForm.h"
#include "bcplus/statements/detail/ImplLawForm.h"
#include "bcplus/statements/detail/TemporalLawForm.h"

namespace bcplus {
namespace statements {


typedef detail::BasicLawForm<
	Statement::Type::LAW_BASIC, 
	elements::Formula,			// F
	elements::Formula,			// if G
	elements::Formula,			// ifcons G2
	elements::Formula,			// after H
	elements::AtomicFormula,	// unless M
	elements::Formula			// where N
	> BasicLaw;

typedef detail::BasicLawForm<
	Statement::Type::LAW_CAUSED, 
	elements::Formula,			// caused F
	elements::Formula,			// if G
	elements::Formula,			// ifcons G2
	elements::Formula,			// after H
	elements::AtomicFormula,	// unless M
	elements::Formula			// where N
	> CausedLaw;

typedef detail::BasicLawForm<
	Statement::Type::LAW_PCAUSED,
	elements::Formula,			// possibly caused F
	elements::Formula,			// if G
	elements::Formula,			// ifcons G2
	elements::Formula,			// after H
	elements::AtomicFormula,	// unless M
	elements::Formula			// where N
	> PossiblyCausedLaw;

typedef detail::ImplLawForm<
	Statement::Type::LAW_IMPL,
	elements::Formula,			// F
	elements::Formula,			// <- G
	elements::Formula 			// where N
	> ImplicationLaw;

typedef detail::DynamicLawForm<
	Statement::Type::LAW_CAUSES,
	elements::AtomicFormula,	// G
	elements::Formula,			// causes F
	elements::Formula,			// if H
	elements::AtomicFormula,	// unless M
	elements::Formula			// where N
	> CausesLaw;

typedef detail::DynamicValueLawForm<
	Statement::Type::LAW_INCREMENTS,
	elements::AtomicFormula,	// G
	elements::Constant,			// increments c
	elements::Term,				// by V
	elements::Formula,			// if H
	elements::AtomicFormula,	// unless M
	elements::Formula			// where N
	> IncrementsLaw;

typedef detail::DynamicValueLawForm<
	Statement::Type::LAW_DECREMENTS,
	elements::AtomicFormula,	// G
	elements::Constant,			// increments c
	elements::Term,				// by V
	elements::Formula,			// if H
	elements::AtomicFormula,	// unless M
	elements::Formula			// where N
	> DecrementsLaw;

typedef detail::DynamicLawForm<
	Statement::Type::LAW_MCAUSE,
	elements::AtomicFormula,	// G
	elements::Formula,			// may cause F
	elements::Formula,			// if H
	elements::AtomicFormula,	// unless M
	elements::Formula			// where N
	> MayCauseLaw;

typedef detail::ConstraintLawForm<
	Statement::Type::LAW_ALWAYS,
	elements::Formula,			// always G
	elements::Formula,			// after H
	elements::AtomicFormula,	// unless M
	elements::Formula			// where N
	> AlwaysLaw;

typedef detail::ConstraintLawForm<
	Statement::Type::LAW_CONSTRAINT,
	elements::Formula,			// constraint G
	elements::Formula,			// after H
	elements::AtomicFormula,	// unless M
	elements::Formula			// where N
	> ConstraintLaw;

typedef detail::ConstraintLawForm<
	Statement::Type::LAW_IMPOSSIBLE,
	elements::Formula,			// impossible G
	elements::Formula,			// after H
	elements::AtomicFormula,	// unless M 
	elements::Formula			// where N
	> ImpossibleLaw;

typedef detail::ConstraintLawForm<
	Statement::Type::LAW_NEVER,
	elements::Formula,			// never G
	elements::Formula,			// after H
	elements::AtomicFormula,	// unless M
	elements::Formula			// where N
	> NeverLaw;

typedef detail::BasicLawForm<
	Statement::Type::LAW_DEFAULT,
	elements::AtomicFormula,	// default c=v
	elements::Formula,			// if G
	elements::Formula,			// ifcons G2
	elements::Formula,			// after H
	elements::AtomicFormula,	// unless M
	elements::Formula			// where N
	> DefaultLaw;

typedef detail::BasicLawForm<
	Statement::Type::LAW_EXOGENOUS,
	elements::Constant,			// exogenous c
	elements::Formula,			// if G
	elements::Formula,			// ifcons G2
	elements::Formula,			// after H
	elements::AtomicFormula,	// unless M
	elements::Formula			// where N
	> ExogenousLaw;

typedef detail::BasicLawForm<
	Statement::Type::LAW_INERTIAL,
	elements::Constant,			// inertial c
	elements::Formula,			// if G
	elements::Formula,			// ifcons G2
	elements::Formula,			// after H
	elements::AtomicFormula,	// unless M
	elements::Formula			// where N
	> InertialLaw;

typedef detail::DynamicConstraintLawForm<
	Statement::Type::LAW_NONEXECUTABLE,
	elements::Formula,			// nonexecutable G
	elements::Formula, 			// if G2
	elements::AtomicFormula,	// unless M
	elements::Formula			// where N
	> NonexecutableLaw;

typedef detail::SimpleLawForm<
	Statement::Type::LAW_RIGID,
	elements::Constant,	// rigid c
	elements::Formula	// where N
	> RigidLaw;

typedef detail::TemporalLawForm<
	Statement::Type::LAW_OBSERVED,
	elements::AtomicFormula, // observed c=v
	parser::Number	// at V
	> ObservedLaw;

typedef detail::TemporalLawForm<
	Statement::Type::LAW_TEMPORAL_CONSTRAINT,
	elements::Formula, // constraint F
	parser::Number	// at V
	> TemporalConstraintLaw;


}}

