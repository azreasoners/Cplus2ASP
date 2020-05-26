#pragma once

#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "bcplus/Location.h"
#include "bcplus/statements/Statement.h"

namespace bcplus {
namespace statements {
namespace detail {

/// Base law form for simple laws containing only a head and where body
template <Statement::Type::type type, typename head_t, typename where_t >
class SimpleLawForm : public Statement {
public:
	/**************************************************************************************/
	/* Public Types */
	/**************************************************************************************/
	
	typedef head_t head_type;
	typedef where_t where_type;


private:

	/**************************************************************************************/
	/* Private Members */
	/**************************************************************************************/
	
	/// Head element
	babb::utils::ref_ptr<const head_t> _head;

	/// 'where' clause
	babb::utils::ref_ptr<const where_t> _where;

public:
	/**************************************************************************************/
	/* Constructors / Destructors */
	/**************************************************************************************/
	SimpleLawForm(head_t const* head, where_t const* where,
		Location const& begin = Location(NULL, 0, 0), Location  
		const& end = Location(NULL, 0, 0));

	virtual ~SimpleLawForm();

	/**************************************************************************************/
	/* Public Methods */
	/**************************************************************************************/

	/// Get the head element
	inline head_t const* head() const 							{ return _head; }

	/// Get the 'where' clause
	inline where_t const* where() const							{ return _where; }

	// inherited
	virtual Statement* copy() const;


};


}}}

#include "bcplus/statements/detail/SimpleLawForm.impl.h"

