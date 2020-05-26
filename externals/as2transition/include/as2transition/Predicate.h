#pragma once

#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "as2transition/Config.h"
#include "as2transition/PredElement.h"



namespace as2transition {

/// Specialized predicate container used primarily for formatting
class Predicate : public PredElement {
public:
	/******************************************************************/
	/* Types */
	/******************************************************************/


	/// The type of constant that this predicate is...
	struct Type {
		enum type {
			ACTION,
			FLUENT,
			EXT_ACTION,
			EXT_FLUENT,
			RIGID,
			UNKNOWN
		};

	};

	/// Static time step
	static const int TIMELESS = -1;


private:
	
	/******************************************************************/
	/* Private Members */
	/******************************************************************/

	/// The type of constant that this is...
	Type::type _type;

	/// The timestep this constant belongs to
	int _timestep;

	/// The inner element for this predicate (or NULL)
	babb::utils::ref_ptr<PredElement> _inner;

	/// The value element for this predicate (or NULL)
	babb::utils::ref_ptr<PredElement> _value;

	/// The timestep element for this predicate (or NULL)
	babb::utils::ref_ptr<PredElement> _ts;

	/// The type of predicate that this is
	int _predmask;

	/// Whether we should use the 'eq' style instead of 'eql' where applicable...
	bool _eq;

public:
	/******************************************************************/
	/* Public Methods */
	/******************************************************************/
	/// Full Constructor
	/// @param base The base name of the predicate
	/// @param args The arguments for the predicate (or NULL)
	/// @param strongNegation Whether this predicate is strongly negated
	Predicate(ReferencedString const* base, ElementList* args = NULL, bool negated = false);


	/// Basic destructor
	virtual ~Predicate();


	/// Gets the timestep for the predicate (or TIMELESS if there isn't one)
	inline int timestep() const									{ return _timestep; }

	/// Gets the type of constant that this is
	inline Type::type constType() const							{ return _type; }

	/// Gets the predicate typemask for this predicate
	inline int predmask() const									{ return _predmask; }


	/// Reformats the predicate according to the format rules
	/// @param config The format rules to apply
	/// @return The newly formatted element or NULL if the format rules specify the element should be removed.
	virtual PredElement * format(Config const* config) const;





};





}




