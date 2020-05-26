#pragma once

#include <list>
#include <string>
#include <map>

#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "bcplus/symbols/VariableSymbol.h"


namespace cplus2asp {
namespace cplus2asp_bin {

/// Tracks the incremental part that we're working with.
struct IPart {
	enum type {
		BASE,				/// #base
		CUMULATIVE,			/// #cumulative t.
		VOLATILE,			/// #volatile t.
		EXTERNAL,			/// #step x.
		NONE				/// doesn't matter
	};
};

typedef ReferencedList<std::string>::type ClauseList;

typedef std::pair<std::string, IPart::type> TranslationStatement;
typedef ReferencedList<TranslationStatement>::type StatementList;


typedef std::map<babb::utils::ref_ptr<const bcplus::symbols::VariableSymbol>, 
	babb::utils::ref_ptr<const ReferencedString> > VariableSubstitutionList;


}}
