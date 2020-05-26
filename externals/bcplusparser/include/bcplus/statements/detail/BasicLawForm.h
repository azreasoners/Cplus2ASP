#pragma once

#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "bcplus/Location.h"
#include "bcplus/statements/Statement.h"

namespace bcplus {
namespace statements {
namespace detail {

/// Base law form for basic laws
template <Statement::Type::type type, typename head_t, typename ifbody_t, typename ifcons_t, typename after_t, typename unless_t, typename where_t>
class BasicLawForm : public Statement {
public:
	/**************************************************************************************/
	/* Public Types */
	/**************************************************************************************/
	
	typedef head_t head_type;
	typedef ifbody_t ifbody_type;
	typedef ifcons_t ifcons_type;
	typedef after_t after_type;
	typedef unless_t unless_type;
	typedef where_t where_type;


private:

	/**************************************************************************************/
	/* Private Members */
	/**************************************************************************************/
	
	/// Head element
	babb::utils::ref_ptr<const head_t> _head;

	/// 'if' clause
	babb::utils::ref_ptr<const ifbody_t> _ifbody;

	// 'ifcons' clause
	babb::utils::ref_ptr<const ifcons_t> _ifcons;

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
	BasicLawForm(head_t const* head, ifbody_t const* ifbody, ifcons_t const* ifcons,
		after_t const* after, unless_t const* unless, where_t const* where,
		Location const& begin = Location(NULL, 0, 0), Location  
		const& end = Location(NULL, 0, 0));

	virtual ~BasicLawForm();

	/**************************************************************************************/
	/* Public Methods */
	/**************************************************************************************/

	/// Get the head element
	inline head_t const* head() const 							{ return _head; }

	/// Get the 'if' clause
	inline ifbody_t const* ifbody() const						{ return _ifbody; }

	/// Get the 'ifcons' clause
	inline ifcons_t const* ifcons() const						{ return _ifcons; }

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

#include "bcplus/statements/detail/BasicLawForm.impl.h"

