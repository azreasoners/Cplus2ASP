/* 
 * Copyright (c) 2010-2013 <Joseph Babb, Michael Cassollary, Joohyung Lee>
 *
 * For information on how to contact the authors, please visit
 *	http://reasoning.eas.asu.edu/cplus2asp
 *
 * This file is part of the cplus2asp system.
 *
 * cplus2asp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * cplus2asp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <list>
#include <iostream>

#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "as2transition/Timestep.h"
#include "as2transition/Predicate.h"
#include "as2transition/PredElement.h"
#include "as2transition/Config.h"


namespace u = babb::utils;

namespace as2transition {


// Adds a predicate to our list of predicates
void Timestep::add(Predicate* newPredicate)
{
	insert(_constants, newPredicate);

	switch (newPredicate->constType()) {
	case Predicate::Type::EXT_FLUENT:
		insert(_extFluents, newPredicate);
		break;


	case Predicate::Type::EXT_ACTION:
		insert(_extActions, newPredicate);
		break;

	case Predicate::Type::ACTION:
		insert(_actions, newPredicate);
		break;

	case Predicate::Type::RIGID:
	case Predicate::Type::UNKNOWN:
	case Predicate::Type::FLUENT:
	default:
		insert(_fluents, newPredicate);
		break;
	}
}


// Simple method used to insert items into a sorted list.
void Timestep::insert(PredList& list, Predicate* pred) {
	iterator it;
	for (it = list.begin(); it != list.end() && **it < *pred; it++);
	list.insert(it, pred);
}


size_t Timestep::output( std::ostream& out ) const {

	size_t counter = 0;

	if (time() == TIMELESS) {
		out << "\t";
	} else {
		// Output time step header and appropriate trailing whitespace.
		out << "\t" << time() << ": ";
	}


	counter += print(out, _fluents);

	if (!_config->separateActions()) {
		counter += print(out, _actions);
	}	

	counter += print(out, _extFluents, 
		(_config->separateExternal()) ? "EXTERNAL FLUENTS" : NULL);

	if (_config->separateActions()) {
		counter += print(out, _actions, "ACTIONS");
	}

	counter += print(out, _extActions, 
		(_config->separateExternal()) ? "EXTERNAL ACTIONS" : NULL);

	out << std::endl;

	return counter;
}

// prints the provided list  and associated label.
size_t Timestep::print(std::ostream& out, PredList const& list, char const* lbl) const {
	std::list<u::ref_ptr<PredElement> > tmp;
	size_t sz;

	for(PredList::const_iterator it = list.begin(); it != list.end(); it++) {
		u::ref_ptr<PredElement> pe = (*it)->format(_config);
		if (pe) tmp.push_back(pe);

	}

	if ((sz = tmp.size())) {
		// print the label.
		if (lbl) out << "\n\n\t" << lbl << ": ";

		// print the predicates
		for(std::list<u::ref_ptr<PredElement> >::const_iterator it = tmp.begin(); it != tmp.end(); it++) {
			out
				<< ((_config->onePredPerLine()) ? "\n\t\t" : " ")
				<< **it;
		}
	}

	return sz;
}


}
