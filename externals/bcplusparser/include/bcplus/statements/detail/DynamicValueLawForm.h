#pragma once

#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "bcplus/Location.h"
#include "bcplus/statements/Statement.h"

namespace bcplus {
namespace statements {
namespace detail {

/// Base law form for basic laws
template <Statement::Type::type type, typename body_t, typename head_t, typename value_t, typename ifbody_t, typename unless_t, typename where_t>
class DynamicValueLawForm : public Statement {
public:
	/**************************************************************************************/
	/* Public Types */
	/**************************************************************************************/
	
	typedef body_t body_type;
	typedef head_t head_type;
	typedef value_t value_type;
	typedef ifbody_t ifbody_type;
	typedef unless_t unless_type;
	typedef where_t where_type;


private:

	/**************************************************************************************/
	/* Private Members */
	/**************************************************************************************/
	
	/// Head element
	babb::utils::ref_ptr<const body_t> _body;

	/// Body element
	babb::utils::ref_ptr<const head_t> _head;

	/// The value to assign the body
	babb::utils::ref_ptr<const value_t> _value;

	/// 'if' clause
	babb::utils::ref_ptr<const ifbody_t> _ifbody;

	/// 'unless' clause
	babb::utils::ref_ptr<const unless_t> _unless;

	/// 'where' clause
	babb::utils::ref_ptr<const where_t> _where;

public:
	/**************************************************************************************/
	/* Constructors / Destructors */
	/**************************************************************************************/
	DynamicValueLawForm(body_t const* body, head_t const* head, value_t const* value, ifbody_t const* ifbody, 
		unless_t const* unless, where_t const* where,
		Location const& begin = Location(NULL, 0, 0), Location  
		const& end = Location(NULL, 0, 0));

	virtual ~DynamicValueLawForm();

	/**************************************************************************************/
	/* Public Methods */
	/**************************************************************************************/

	/// Get the body element
	inline body_t const* body() const 							{ return _body; }

	/// Get the head element
	inline head_t const* head() const							{ return _head; }

	/// Get the value element
	inline value_t const* value() const							{ return _value; }

	/// Get the 'if' clause
	inline ifbody_t const* ifbody() const						{ return _ifbody; }

	/// Get the 'unless' clause
	inline unless_t const* unless() const						{ return _unless; }

	/// Get the 'where' clause
	inline where_t const* where() const							{ return _where; }

	// inherited
	virtual Statement* copy() const;


};


}}}

#include "bcplus/statements/detail/DynamicValueLawForm.impl.h"

