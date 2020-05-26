
#include <string>

#include "babb/utils/memory.h"
#include "babb/utils/utils.h"
#include "memwrappers.h"

#include "as2transition/PredElement.h"
#include "as2transition/Predicate.h"
#include "as2transition/Config.h"

namespace u = babb::utils;

namespace as2transition {


Predicate::Predicate(ReferencedString const* b, ElementList* args, bool negated)
	: PredElement(PredElement::Type::PREDICATE, b, args),
	  _type(Type::UNKNOWN), _timestep(TIMELESS), _predmask(0),
	  _eq(false) {

	// Figure out what format we're in....

	bool is_h = (*base() == "h");
	bool is_o = ((*base() == "o") || (*base() == "occ"));

	if (negated) _predmask |= Config::PredType::STRONG_NEGATION;

	if (is_h || is_o) {
		// h(...,t) / o(...,t)...

		bool good = false;

		switch (arity()) {
		case 1:
			// this may be or h(eql(c,v)) ....
			if (*get(0)->base()	== "eq" && get(0)->arity() == 2) {
				_eq = true;
				_inner = get(0)->get(0);
				_value = get(0)->get(1);
				good = true;
			} else if (*get(0)->base() == "eql" && get(0)->arity() == 2) {
				_inner = get(0)->get(0);
				_value = get(0)->get(1);
				good = true;
			} 

			break;

		case 2:
			// this is probably h(eql(c,v),t) or o(eql(c,v),t)
			if (*get(0)->base()	== "eq" && get(0)->arity() == 2) {
				_eq = true;
				_inner = get(0)->get(0);
				_value = get(0)->get(1);
				good = true;
				_ts = get(1);
			} else if (*get(0)->base() == "eql" && get(0)->arity() == 2) {
				_inner = get(0)->get(0);
				_value = get(0)->get(1);
				good = true;
				_ts = get(1);
			} 

			break;

		case 3:
			// this is probably h(c,v,t) or o(c,v,t)
			_inner = get(0);
			_value = get(1);
			_ts = get(2);
			good = true;
			break;
	
		default:
			// false alarm, we don't recognize the arity...
			break;

		}

		if (good) {
			if (_ts) _type = (is_h ? Type::FLUENT : Type::ACTION);
			else _type = Type::RIGID;

		}

	} else {
		// perhaps c_x_x_ or e_x_x ?
		if (base()->find("c_") == 0 || base()->find("e_") == 0) {
			Type::type t = Type::UNKNOWN;

			size_t l1 = base()->find("_", 2);
			size_t l2 = (l1 != std::string::npos ? base()->find("_",l1+1) : std::string::npos);
			if (l1 != std::string::npos && l2 != std::string::npos) {
				// figure out the constant type
				switch (base()->at(2)) {
				case 'a': t = Type::ACTION;	break;
				case 'r': t = Type::RIGID;	break;
				case 'f': t = Type::FLUENT; break;
				default:					break;

				}

				// and the arity...
				std::string arity_string = base()->substr(l1+1,l2-l1-1);
				size_t iarity;
				if (!u::fromString(arity_string.c_str(), iarity)) {
					t = Type::UNKNOWN;
				}
				
				// verify arity...
				if (t == Type::RIGID && iarity + 1 != arity()) t = Type::UNKNOWN;
				else if (t != Type::RIGID && iarity + 2!= arity()) t = Type::UNKNOWN;

				
				if (t != Type::UNKNOWN) {
					// success so far... parse the arguments...
					_type = t;
					_value = get(iarity);
					if (_type != Type::RIGID) _ts = get(iarity+1);
					
					// construct a new inner type...
					u::ref_ptr<ElementList> newargs = new ElementList();
					for (size_t i = 0; i < iarity; i++) {
						newargs->push_back(get(i));
					}
					_inner = new PredElement(PredElement::Type::OBJECT, base(), newargs);


					if (base()->at(0) == 'e') {
						// external constant...
						if (t == Type::ACTION) t = Type::EXT_ACTION;
						else t = Type::EXT_FLUENT;
					}

				}

			}
		}

		// contrib_* ?
		else if (base()->find("contrib_") == 0) {
			// this looks like a contribution...
	
			if (arity() == 4) {
				// contrib(c,a,v,t)
				_type = Type::ACTION;
				_value = get(2);
				_ts = get(3);
					
				// construct a new inner type...
				u::ref_ptr<ElementList> newargs = new ElementList();
				newargs->push_back(get(0));
				newargs->push_back(get(1));
				_inner = new PredElement(PredElement::Type::OBJECT, new ReferencedString("contribution"), newargs);
			}

		}
	}


	// almost done... now we just need to extract the timestep and some additional information about the inner type...

	// time stamp...
	if (_ts && _ts->arity() == 0) {
		u::fromString(_ts->base()->c_str(), _timestep);
	} 


	if (_value) {
		if (_value->arity() == 0
			&& (*_value->base() == "none" || *_value->base() == "o_none"
				|| *_value->base() == "false" || *_value->base() == "o_false"))
			_predmask |= Config::PredType::NEGATIVE;
		else _predmask |= Config::PredType::POSITIVE;
	} else _predmask |= Config::PredType::UNKNOWN;

	if (_inner) {
		if (*_inner->base() == "contribution") _predmask |= Config::PredType::CONTRIBUTION;
		else if (_inner->base()->find("x_") == 0) _predmask |= Config::PredType::XPREDICATE;

	} else _predmask |= Config::PredType::UNKNOWN;


}
Predicate::~Predicate() {
	// Intentionally left blank
}



PredElement* Predicate::format(Config const* config) const {
	/// Cull things we shouldn't show...

	if (!config->show(predmask())) return NULL;

	// If this predicate isn't well formed, just return this
	if (!_value || !_inner) return PredElement::format(config);


	u::ref_ptr<ElementList> args = new ElementList();
	u::ref_ptr<PredElement> ret;


	switch (config->format()) {
	case Config::PredFormat::RAW:
		// just return this...
		ret = PredElement::format(config);
		if ((predmask() & Config::PredType::STRONG_NEGATION)) 
			ret = new PredElement(PredElement::Type::OBJECT, new ReferencedString("~"), ret);
		return ret.release();

	case Config::PredFormat::INNER:
		args->push_back(_inner->format(config));
		args->push_back(_value->format(config));
		ret = new PredElement(PredElement::Type::OBJECT, new ReferencedString((_eq ? "eq" : "eql")), args);
		if ((predmask() & Config::PredType::STRONG_NEGATION)) 
			ret = new PredElement(PredElement::Type::OBJECT, new ReferencedString("~"), ret);
		return ret.release();

	case Config::PredFormat::SHORT:
		if (_value->arity() == 0 && (*_value->base() == "true" || *_value->base() == "o_true"))	return _inner->format(config);
		if (_value->arity() == 0 && (*_value->base() == "false" || *_value->base() == "o_false")) {
			args->push_back(_inner->format(config));
			return new PredElement(PredElement::Type::OBJECT, new ReferencedString("-"),
				((predmask() & Config::PredType::STRONG_NEGATION) ? 
					  new PredElement(PredElement::Type::OBJECT, new ReferencedString("~"), _inner->format(config))
					: _inner->format(config)));
		}

		/* no break */
	case Config::PredFormat::EQL:
		args->push_back(_inner->format(config));
		args->push_back(_value->format(config));
		ret = new PredElement(PredElement::Type::OBJECT, new ReferencedString("="), args);
		if ((predmask() & Config::PredType::STRONG_NEGATION)) 
			ret = new PredElement(PredElement::Type::OBJECT, new ReferencedString("~"), ret);
		return ret.release();
	default:
		// just return this...
		ret = PredElement::format(config);
		if ((predmask() & Config::PredType::STRONG_NEGATION)) 
			ret = new PredElement(PredElement::Type::OBJECT, new ReferencedString("~"), ret);
		return ret.release();
	}



}

}
