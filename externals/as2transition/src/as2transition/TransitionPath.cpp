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

#include <vector>
#include <iostream>

#include "as2transition/TransitionPath.h"
#include "as2transition/Timestep.h"
#include "as2transition/Config.h"

namespace as2transition {

// Adds the predicate to the transition path.
bool TransitionPath::add(Predicate* pred) {
	size_t time = pred->timestep();
	Timestep* ts = step(time);
	if (ts) {
		ts->add(pred);
		return false;
	} else {
		size_t max = length();
		while (max <= time) {
			_steps.push_back(new Timestep(_config,max++));
		}
		_steps[time]->add(pred);
		return true;
	}
}

// Prints the path to the stream
std::ostream& TransitionPath::output(std::ostream& out) const {
	if (_timeless->num()) {
		out << *_timeless;
		out << std::endl;
	}

	for (Timestep const* ts : *this) {
		out << *ts;
		out << std::endl;
	}
	return out;
}


}; /* end namespace as2transition */
