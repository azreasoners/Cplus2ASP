#pragma once

#include <string>
#include <ostream>

#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>

#include "babb/utils/memory.h"

#include "bcplus/DomainType.h"

namespace bcplus {
namespace symbols {

/**
 * @brief A base symbol class for building symbol tables.
 */
class Symbol : public babb::utils::Referenced
{
public:
	/************************************************************************/
	/* Public Types */
	/************************************************************************/
	/// Container for the types of symbols
	struct Type {
		/// Enumeration of possible values
		enum type {
			SORT		= 0x0001,
			CONSTANT	= 0x0002,
			VARIABLE	= 0x0004,
			OBJECT		= 0x0008,
			MACRO		= 0x0010,
			QUERY		= 0x0020,
			RANGE		= 0x0040,
			_LARGEST_	= 0x0040,			///< placeholder for the largest type value	
			ERR_INVALID_SYMBOL = 0x1000		///< placeholder for invalid value
		};


		/// Interesting masks of multiple symbol types
		enum Mask {
			M_ANY		= ~0,
			M_TERM		= VARIABLE | OBJECT | MACRO | CONSTANT,
			M_FORMULA	= CONSTANT | MACRO
		};

		/// Conversion to a cstring
		static char const* cstr(type v);

		/// Conversion from a cstring
		static type val(char const* c);

	};


private:
	/************************************************************************/
	/* Private Members */
	/************************************************************************/
	Type::type _type;
	size_t _arity;
	babb::utils::ref_ptr<const babb::utils::ReferencedString> _base;
	babb::utils::ref_ptr<const babb::utils::ReferencedString> _name;
	bool _good;

	/// A convenient place to put additional metadata
	babb::utils::ref_ptr<Referenced> _metadata;

public:
	/************************************************************************/
	/* Static Functions */
	/************************************************************************/
	/// Generate the symbol's name from its base and arity
	static std::string genName(std::string const& base, size_t arity = 0);

	/************************************************************************/
	/* Constructor / Destructor */
	/************************************************************************/
	/// Basic Constructor
	/// @param type The type of symbol being instantiated
	/// @param base The base name of the symbol
	/// @param arity The arity of symbol
	Symbol(Type::type type, babb::utils::ReferencedString const* base, size_t arity);
	
	/// Attempts to load the symbol from the property tree node.
	/// @param type The type of symbol being instantiated
	/// @param node The node to load the symbol from
	/// @param err An error stream to write to or NULL.
	/// Sets the symbol's good flag if it was successful.
	Symbol(Type::type type, boost::property_tree::ptree const& node, std::ostream* err = NULL);

	/// Destructor stub
	virtual ~Symbol();
	
	/************************************************************************/
	/* Public Functions */
	/************************************************************************/


	/// Get the symbol type
	inline Type::type type() const					{ return _type; }

	/// Get the symbol type as a cstring
	inline char const* typeString() const			{ return Type::cstr(type()); }

	/// Get the arity of the symbol
	inline size_t arity() const						{ return _arity; }

	/// Get the base name of the symbol
	inline babb::utils::ReferencedString const* base() const		{ return _base; }
	
	/// Get the full name of the symbol
	inline babb::utils::ReferencedString const* name() const		{ return _name; }

	/// Determines if the symbol has been successfully loaded.
	inline bool good() const						{ return _good; }

	/// Determine if this symbol matches the provided detail
	/// @param b The base name to match against.
	/// @param a The arity to match against
	inline bool match(std::string const& b, size_t a) const
													{return (a == arity()) && (b == *base()); }
	/// Determine if this symbol matches the provided detail
	/// @param n The full name of the symbol to match against (base/arity)
	inline bool match(std::string const&n) const	{ return n == *name(); }
	
	/// Get/set the metadata
	inline Referenced* metadata() const					{ return const_cast<Referenced*>(_metadata.get()); }
	template <typename T>
	inline T* metadata() const							{ return const_cast<T*>((T const*)_metadata.get()); }
	
	inline Referenced* metadata() 						{ return _metadata; }
	template <typename T>
	inline T* metadata() 								{ return (T*)_metadata; }
	inline void metadata(Referenced* data)				{ _metadata = data; }
	

	/// Check to see if two symbols are equal
	/// @param other The other symbol to compare this one to
	/// @return True if they are equal, false otherwise.
	virtual bool operator==(Symbol const& other) const;

	inline bool operator!=(Symbol const& other) const{ return !(*this == other); }

	/// Outputs the symbol to the provided property tree node
	/// @param node The node to write to
 	virtual void save(boost::property_tree::ptree& node) const;

	/// Determine the domain type of this symbol.
	virtual DomainType::type domainType() const = 0;

protected:

	/// Set the good flag
	inline void good(bool l)						{ _good = l; }

};

}}
