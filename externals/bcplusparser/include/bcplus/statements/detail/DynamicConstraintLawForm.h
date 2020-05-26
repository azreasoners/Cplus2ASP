#pragma once

#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "bcplus/Location.h"
#include "bcplus/statements/Statement.h"

namespace bcplus {
namespace statements {
namespace detail {

/// Base law form for basic laws
template <Statement::Type::type type, typename body_t, typename ifbody_t, typename unless_t, typename where_t>
class DynamicConstraintLawForm : public Statement {
public:
	/**************************************************************************************/
	/* Public Types */
	/**************************************************************************************/
	
	typedef body_t body_type;
	typedef ifbody_t ifbody_type;
	typedef unless_t unless_type;
	typedef where_t where_type;


private:

	/**************************************************************************************/
	/* Private Members */
	/**************************************************************************************/
	
	/// main body
	babb::utils::ref_ptr<const body_t> _body;

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
	DynamicConstraintLawForm(body_t const* body, ifbody_t const* ifbody,
		unless_t const* unless, where_t const* where,
		Location const& begin = Location(NULL, 0, 0), Location  
		const& end = Location(NULL, 0, 0));

	virtual ~DynamicConstraintLawForm();

	/**************************************************************************************/
	/* Public Methods */
	/**************************************************************************************/

	/// Get the body of the law
	inline body_t const* body() const							{ return _body; }

	// Get the 'if' clause
	inline ifbody_t const* ifbody() const						{ return _ifbody; }

	/// Get the 'unless' clause
	inline unless_t const* unless() const						{ return _unless; }

	/// Get the 'where' clause
	inline where_t const* where() const							{ return _where; }

	// inherited
	virtual Statement* copy() const;


};


}}}

#include "bcplus/statements/detail/DynamicConstraintLawForm.impl.h"

