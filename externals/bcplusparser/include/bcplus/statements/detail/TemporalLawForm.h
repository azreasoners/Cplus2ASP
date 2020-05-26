#pragma once

#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "bcplus/Location.h"
#include "bcplus/statements/Statement.h"

namespace bcplus {
namespace statements {
namespace detail {

/// Temporal laws with a head and a time step.
template <Statement::Type::type type, typename head_t, typename at_t >
class TemporalLawForm : public Statement {
public:
	/**************************************************************************************/
	/* Public Types */
	/**************************************************************************************/
	
	typedef head_t head_type;
	typedef at_t at_type;


private:

	/**************************************************************************************/
	/* Private Members */
	/**************************************************************************************/
	
	/// Head element
	babb::utils::ref_ptr<const head_t> _head;

	/// 'at' clause
	babb::utils::ref_ptr<const at_t> _at;

public:
	/**************************************************************************************/
	/* Constructors / Destructors */
	/**************************************************************************************/
	TemporalLawForm(head_t const* head, at_t const* at,
		Location const& begin = Location(NULL, 0, 0), Location  
		const& end = Location(NULL, 0, 0));

	virtual ~TemporalLawForm();

	/**************************************************************************************/
	/* Public Methods */
	/**************************************************************************************/

	/// Get the head element
	inline head_t const* head() const 							{ return _head; }

	/// Get the 'at' clause
	inline at_t const* at() const							{ return _at; }

	// inherited
	virtual Statement* copy() const;


};


}}}

#include "bcplus/statements/detail/TemporalLawForm.impl.h"

