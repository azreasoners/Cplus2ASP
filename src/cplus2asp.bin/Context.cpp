
#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "types.h"
#include "Context.h"

namespace cplus2asp {
namespace cplus2asp_bin {

Context::Context(Configuration const* conf, Position::type pos, IPart::type ipart, StatementList* preStatements, ClauseList* extraClauses, ClauseList* existsVars, bool neg, ReferencedString const* timestamp, VariableSubstitutionList const* subs)
	: _config(conf), _pos(pos), _neg(neg), _ts(timestamp), _extraClauses(extraClauses), _preStmts(preStatements), _existsVars(existsVars), _ipart(ipart) {

	if (subs) _subs.insert(subs->begin(), subs->end());

	if (!timestamp) {
		switch (ipart) {
		case IPart::CUMULATIVE:	
		case IPart::VOLATILE:		_ts = config()->ts(Configuration::TS::STATIC);		break;

		case IPart::NONE:
		case IPart::BASE:			
		default:					_ts = config()->ts(Configuration::TS::ZERO); 		break;
		}
	}
}

Context::~Context() {
	/* Intentionally left blank */
}

Context* Context::mkPos(Position::type pos, bool negated) {
	return new Context(_config, pos, _ipart, _preStmts, _extraClauses, _existsVars, negated, _ts, &_subs);
}

Context* Context::mkPos(Position::type pos, IPart::type ipart, bool negated) {
	return new Context(_config, pos, ipart, _preStmts, _extraClauses, _existsVars, negated, _ts, &_subs);
}

Context* Context::mkTime(ReferencedString const* ts) {
	return new Context(_config, _pos, _ipart, _preStmts, _extraClauses, _existsVars, _neg, ts, &_subs);
}


Context* Context::mkBinds(ClauseList* extraClauses, ClauseList* existsVars) {
	return new Context(_config, _pos, _ipart, _preStmts, extraClauses, existsVars, _neg, _ts, &_subs);
}

Context* Context::mkBindStmts(StatementList* preStatements) {
	return new Context(_config, _pos, _ipart, preStatements, _extraClauses, _existsVars, _neg, _ts, &_subs);
}

Context* Context::mkVarSubstitutions(VariableSubstitutionList const& subs) {
	Context* ret = new Context(_config, _pos, _ipart, _preStmts, _extraClauses, _existsVars, _neg, _ts, &_subs);

	for(VariableSubstitutionList::const_iterator it = subs.begin(); it != subs.end(); it++) {
		ret->_subs[it->first] = it->second;
	}
	return ret;
}


}}

