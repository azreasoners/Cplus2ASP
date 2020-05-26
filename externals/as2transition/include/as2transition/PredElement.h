#pragma once

#include <iostream>
#include <list>

#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "as2transition/Config.h"

namespace as2transition {


/// Base class for predicate/object elements
class PredElement : public babb::utils::Referenced {
public:
	/***************************************************************/
 	/* Types */
	/***************************************************************/

	


	/// The type of element that this is...
	struct Type {
		enum type {	
			PREDICATE,
			OBJECT,
			INTEGER,
			STRING_LITERAL
		};
	};



	typedef ReferencedVector<babb::utils::ref_ptr<PredElement> >::type ElementList;
	typedef ElementList::iterator iterator;
	typedef ElementList::const_iterator const_iterator;


private:

	/**************************************************************/
	/* Private Members */
	/**************************************************************/
	/// The basename of this element
	babb::utils::ref_ptr<const ReferencedString> _base;

	/// The arguments of this element
	babb::utils::ref_ptr<ElementList> _args;

	/// The type of element that this is
	Type::type _type;

public:

	/// Basic constructor
	/// @param type The type of element that this is
	/// @param base The base name of this element
	/// @param args The arguments for this element or NULL if the element is nullary
	PredElement(Type::type type, ReferencedString const* base, ElementList* args = NULL);
	
	/// Basic constructor for unary predelement
	/// @param type The type of element that this is
	/// @param base The base name of this element
	/// @param arg The argument for this element
	PredElement(Type::type type, ReferencedString const* base, PredElement* arg);


	/// Basic Destructor
	virtual ~PredElement();


	/// Gets the base name of the element
	inline ReferencedString const* base() const						{ return _base; }

	/// Gets the type of the element
	inline Type::type type() const									{ return _type; }

	/// Get the number of arguments the element has
	inline size_t arity() const										{ return _args->size(); }


	/// Iterate over the arguments
	inline iterator begin() 										{ return _args->begin(); }
	inline const_iterator begin() const								{ return _args->begin(); }
	inline iterator end() 											{ return _args->end(); }
	inline const_iterator end() const								{ return _args->end(); }


	/// Get a specific argument
	inline PredElement* get(size_t i)								{ return (*_args)[i]; }
	inline PredElement const* get(size_t i)	const					{ return (*_args)[i]; }
	inline PredElement* operator[](int i)							{ return (*_args)[i]; }
	inline PredElement const* operator[](int i)	const				{ return (*_args)[i]; }

	/// comparison operators
	bool operator<(PredElement const& other) const;
	bool operator>(PredElement const& other) const;
	bool operator==(PredElement const& other) const;
	bool operator!=(PredElement const& other) const;



	/// Reformat the element according the provided format rules
	/// @param config How the element should be formatted.
	/// @return The newline formatted element or NULL if the format rules specify the element should be removed
	virtual PredElement* format(Config const* config) const;


};

/// Outputs the predicate element to the provided stream...
std::ostream& operator<<(std::ostream& out, PredElement const& elem);




}
