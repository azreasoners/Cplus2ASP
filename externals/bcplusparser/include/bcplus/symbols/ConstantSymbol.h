#pragma once

#include <string>
#include <ostream>

#include <boost/property_tree/ptree.hpp>

#include "babb/utils/memory.h"

#include "bcplus/DomainType.h"
#include "bcplus/symbols/Symbol.h"
#include "bcplus/symbols/detail/BaseSymbol.h"

namespace bcplus {
namespace symbols {

class SortSymbol;
class Resolver;

class ConstantSymbol : public detail::BaseSymbol {
public:
	/*************************************************************************************/
	/* Public Types */
	/*************************************************************************************/
	/// The different types a constant can be
	struct Type {
		enum type {
			ABACTION 				= 0x0001,
			ACTION 					= 0x0002,
			ADDITIVEFLUENT 			= 0x0004,
			ADDITIVEACTION 			= 0x0008,
			ATTRIBUTE 				= 0x0010,
			EXTERNALACTION 			= 0x0020,
			EXTERNALFLUENT 			= 0x0040,
			EXOGENOUSACTION			= 0x0080,
			INERTIALFLUENT			= 0x0100,
			RIGID					= 0x0200,
			SDFLUENT				= 0x0400,
			SIMPLEFLUENT			= 0x0800,
			ERR_UNKNOWN				= 0x0000		///< Dummy placeholder for an invalid type
		};

		enum mask {
			M_REGULAR_ACTIONS = ACTION | ADDITIVEACTION | EXOGENOUSACTION,
			M_ACTIONS = ABACTION | ACTION | ADDITIVEACTION | ATTRIBUTE | EXTERNALACTION | EXOGENOUSACTION,
			M_FLUENTS = ADDITIVEFLUENT | EXTERNALFLUENT | INERTIALFLUENT | SDFLUENT | SIMPLEFLUENT,
			M_SIMPLEFLUENTS = SIMPLEFLUENT | INERTIALFLUENT | ADDITIVEFLUENT,
			M_ADDITIVE = ADDITIVEACTION | ADDITIVEFLUENT,
			M_EXTERNAL = EXTERNALACTION | EXTERNALFLUENT,
			M_NON_RIGID = ~RIGID
		};

		/// Convert to cstring
		static char const* cstr(type val);

		/// Convert cstring to enum value
		static type value(char const* val);

	};

private:
	/*************************************************************************************/
	/* Private Members */
	/*************************************************************************************/
	/// The sort this constant ranges over
	babb::utils::ref_ptr<const SortSymbol> _sort;

	/// The constant type
	Type::type _type;

public:
	/// Basic constructor
	/// @param type The type that this constant is
	/// @param base The name of this object
	/// @param sort The sort that this constant ranges over
	/// @param args The sorts for each of the arguments for this object.
	ConstantSymbol(Type::type type, ReferencedString const* base, SortSymbol const* sort, SortList const* args = NULL);

	/// Loads the object from the property tree node
	/// @param node The node to load the symbol from
	/// @param err An error strem to write to (or NULL)
	/// Sets the symbol's good flag if it was successful
	ConstantSymbol(boost::property_tree::ptree const& node, Resolver const* resolver, std::ostream* err = NULL);

	/// Destructor stub
	virtual ~ConstantSymbol();

	/// Get the sort this symbol ranges over
	SortSymbol const* sort() const						{ return _sort; }

	/// Get the type that this constant is
	Type::type constType() const						{ return _type; }

	/// Get the string for the type taht this constant is
	char const* constTypeString() const					{ return Type::cstr(_type); }

	// inherited
	virtual bool operator==(Symbol const& other) const;
	virtual DomainType::type domainType() const;
	virtual void save(boost::property_tree::ptree& node) const;

};

}}
