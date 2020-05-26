#pragma once

#include <ostream>
#include <string>
#include <list>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>

#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "bcplus/Location.h"
#include "bcplus/elements/Element.h"


namespace bcplus {
namespace elements {
namespace detail {

/// Version of AnonymousElement with no arguments
/// @param BaseType The base class for this template
/// @param type The type flag to indicate the specialization of the type
template <typename BaseType, int type>
class AnonymousElement_bare : public BaseType
{


private:

	babb::utils::ref_ptr<const ReferencedString> _base;

public:
    /****************************************************************************/
    /* Constructor / Destructor */
    /****************************************************************************/

    /// Full constructor
	/// @param base The base name of the element.
    /// @param begin The beginning location of this element
    /// @param end The ending location of this element
    /// @param parens Whether the element is surrounded by parentheses
    AnonymousElement_bare(ReferencedString const* base, Location const& begin = Location(NULL, 0, 0), Location const& end = Location(NULL, 0, 0), bool parens = false);

    /// Destructor stub
    virtual ~AnonymousElement_bare();


    /****************************************************************************/
    /* Public Members */
    /****************************************************************************/

	/// Get the base name of this element
	inline ReferencedString const* base() const 		{ return _base; }

	// Inherited from element
	virtual Element* copy() const;
	virtual void output(std::ostream& out) const;
	virtual DomainType::type domainType() const;
};

/// Version of AnonymousElement with arguments
/// @param BaseType The base class for this template
/// @param type The type flag to indicate the specialization of the type
/// @param ArgType The type that the arguments are allowed to take.
template <typename BaseType, int type, typename ArgType>
class AnonymousElement : public AnonymousElement_bare<BaseType, type>
{
public:
    /****************************************************************************/
    /* Public Types */
    /****************************************************************************/

	/// Argument type aliases
	typedef ArgType Argument;
	typedef typename ReferencedVector<babb::utils::ref_ptr<const Argument> >::type ArgumentList;
	    

	/// Argument iterators
	typedef typename ArgumentList::iterator iterator;
	typedef typename ArgumentList::const_iterator const_iterator;

private:
	/****************************************************************************/
    /* Private Members */
    /****************************************************************************/

	babb::utils::ref_ptr<const ArgumentList> _args;

public:

	/****************************************************************************/
    /* Constructor / Destructor */
    /****************************************************************************/

    /// Full constructor
	/// @param base The base name of the element.
	/// @param args The argument list for this identifier.
    /// @param begin The beginning location of this element
    /// @param end The ending location of this element
    /// @param parens Whether the element is surrounded by parentheses
    AnonymousElement(ReferencedString const* base, ArgumentList const* args, Location const& begin = Location(NULL, 0, 0), Location const& end = Location(NULL, 0, 0), bool parens = false);

    /// Destructor stub
    virtual ~AnonymousElement();

    /****************************************************************************/
    /* Public Members */
    /****************************************************************************/

	/// Get the number of arguments
	inline size_t arity() const							{ return _args->size(); }

	/// Argument list iterators
	inline iterator begin() 							{ return _args->begin(); }
	inline const_iterator begin() const					{ return _args->begin(); }
	inline iterator end()								{ return _args->end(); }
	inline const_iterator end() const					{ return _args->end(); }

	// Inherited from element
	virtual Element* copy() const;
	virtual void output(std::ostream& out) const;

protected:

	/// The argument list
	inline ArgumentList const* args() const				{ return _args; }

};

}}}


#include "bcplus/elements/detail/AnonymousElement.impl.h"

