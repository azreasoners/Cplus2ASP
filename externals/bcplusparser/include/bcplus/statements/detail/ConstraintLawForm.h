#pragma once

#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "bcplus/Location.h"
#include "bcplus/statements/Statement.h"

namespace bcplus {
namespace statements {
namespace detail {

/// Base law form for basic laws
template <Statement::Type::type type, typename body_t, typename after_t, typename unless_t, typename where_t>
class ConstraintLawForm : public Statement {
public:
	/**************************************************************************************/
	/* Public Types */
	/**************************************************************************************/
	
	typedef body_t body_type;
	typedef after_t after_type;
	typedef unless_t unless_type;
	typedef where_t where_type;


private:

	/**************************************************************************************/
	/* Private Members */
	/**************************************************************************************/
	
	/// the statement body
	babb::utils::ref_ptr<const body_t> _body;

	/// 'after' clause
	babb::utils::ref_ptr<const after_t> _after;

	/// 'unless' clause
	babb::utils::ref_ptr<const unless_t> _unless;

	/// 'where' clause
	babb::utils::ref_ptr<const where_t> _where;

public:
	/**************************************************************************************/
	/* Constructors / Destructors */
	/**************************************************************************************/
	ConstraintLawForm(body_t const* body, 
		after_t const* after, unless_t const* unless, where_t const* where,
		Location const& begin = Location(NULL, 0, 0), Location  
		const& end = Location(NULL, 0, 0));

	virtual ~ConstraintLawForm();

	/**************************************************************************************/
	/* Public Methods */
	/**************************************************************************************/

	/// Get the 'if' clause
	inline body_t const* body() const							{ return _body; }

	/// Get the 'after' clause
	inline after_t const* after() const							{ return _after; }

	/// Get the 'unless' clause
	inline unless_t const* unless() const						{ return _unless; }

	/// Get the 'where' clause
	inline where_t const* where() const							{ return _where; }

	// inherited
	virtual Statement* copy() const;


};


}}}

#include "bcplus/statements/detail/ConstraintLawForm.impl.h"

