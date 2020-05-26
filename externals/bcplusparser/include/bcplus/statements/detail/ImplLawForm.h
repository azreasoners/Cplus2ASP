#pragma once

#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "bcplus/Location.h"
#include "bcplus/statements/Statement.h"

namespace bcplus {
namespace statements {
namespace detail {

/// Base law form for simple implications with a head and body (and where)
template <Statement::Type::type type, typename head_t, typename body_t, typename where_t >
class ImplLawForm : public Statement {
public:
	/**************************************************************************************/
	/* Public Types */
	/**************************************************************************************/
	
	typedef head_t head_type;
	typedef body_t body_type;
	typedef where_t where_type;


private:

	/**************************************************************************************/
	/* Private Members */
	/**************************************************************************************/
	
	/// Head element
	babb::utils::ref_ptr<const head_t> _head;

	/// Body element
	babb::utils::ref_ptr<const body_t> _body;

	/// 'where' clause
	babb::utils::ref_ptr<const where_t> _where;

public:
	/**************************************************************************************/
	/* Constructors / Destructors */
	/**************************************************************************************/
	ImplLawForm(head_t const* head, body_t const* body, where_t const* where,
		Location const& begin = Location(NULL, 0, 0), Location  
		const& end = Location(NULL, 0, 0));

	virtual ~ImplLawForm();

	/**************************************************************************************/
	/* Public Methods */
	/**************************************************************************************/

	/// Get the head element
	inline head_t const* head() const 							{ return _head; }

	/// Get the body element
	inline body_t const* body() const							{ return _body; }

	/// Get the 'where' clause
	inline where_t const* where() const							{ return _where; }

	// inherited
	virtual Statement* copy() const;


};


}}}

#include "bcplus/statements/detail/ImplLawForm.impl.h"

