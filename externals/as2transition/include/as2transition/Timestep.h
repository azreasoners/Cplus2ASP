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

#pragma once

#include <iostream>
#include <list>
#include <string>

#include "babb/utils/memory.h"


#include "as2transition/Config.h"
#include "as2transition/Predicate.h"

namespace as2transition {

/**
 * Holds information about the predicates that are true at a given time step.
 */
class Timestep : public babb::utils::Referenced
{
public:
	/***************************************************************************/
	/* Types / Constants */
	/***************************************************************************/
	typedef std::list<babb::utils::ref_ptr<Predicate> > PredList;
	typedef PredList::iterator iterator;
	typedef PredList::const_iterator const_iterator;

	/// Time value to indicate that the timestep is not associated with a step in
	/// the transition system.
	static const size_t TIMELESS = Predicate::TIMELESS;


private:
	/***************************************************************************/
	/* Members */
	/***************************************************************************/
	size_t _time; 							///< The time step this structure represents in the solution.
	babb::utils::ref_ptr<const Config> _config;	///< AS2transition configuration information...

	PredList _constants;				///< Complete list of all constants associated with this timestep.

	PredList _fluents; 					///< The fluents that are true at this time step.
	PredList _extFluents; 				///< The static abnormalities that are exhibited at this time step.
	
	PredList _actions; 					///< The actions that occurred in the next transition at this time step.
	PredList _extActions; 				///< The dynamic abnormalities that occurred in the next transition.
public:


	/***************************************************************************/
	/* Constructors / Destructors */
	/***************************************************************************/

	/// Constructor.
	/// @param config The AS2Transition configuration used to format the timestep.
	/// @param time The time stamp associated with this timestep (or TIMELESS).
	inline Timestep(Config const* config, size_t time = TIMELESS)
		: _time(time), _config(config)
	{ /* Intentionally left blank */ }
	
	/// Destructor.  Deallocated associated predicates.
	virtual inline ~Timestep() { 
		// intentionally left blank
	}
	
	/** 
	 * Accessor for timeStep.
	 * @return The value of timeStep.
	 */
	inline size_t time() const 							{ return _time; }

	/** 
	 * Adds a predicate to our list of predicates.
	 * Automatically keeps the predicates sorted.
	 * @param newPredicate - The predicate object to add.
	 */
	void add(Predicate* newPredicate);
	
	/// Gets an iterator for the beginning of the list of all constants associated with the timestep.
	inline const_iterator begin() const 				{ return _constants.begin(); }
	/// Gets an iterator for the end of the list of all constants associated with the timestep.
	inline const_iterator end() const 					{ return _constants.end(); }
	/// Gets the number of constants in the timestep.
	inline size_t num() const							{ return _constants.size(); }

	/// Gets an iterator for the beginning of the list of fluents in the timestep.
	inline const_iterator beginFluents() const 			{ return _fluents.begin(); }
	/// Gets an iterator for the end the list of fluents in the timestep.
	inline const_iterator endFluents() const 			{ return _fluents.end(); }
	/// Gets the number of fluents in the timestep.
	inline size_t numFluents() const					{ return _fluents.size(); }

	/// Gets an iterator for the beginning of the list of static abnormalities in the timestep.
	inline const_iterator beginExtFluents() const 		{ return _extFluents.begin(); }
	/// Gets an iterator for the end the list of static abnormalities in the timestep.
	inline const_iterator endExtFluents() const 		{ return _extFluents.end(); }
	/// Gets the number of static abnormalities in teh timestep
	inline size_t numExtFluents() const					{ return _extFluents.size(); }
	

	/// Gets an iterator for the beginning of the list of actions occurring in the transition.
	inline const_iterator beginActions() const 			{ return _actions.begin(); }
	/// Gets an iterator for the beginning of the list of actions occurring in the transition.
	inline const_iterator endActions() const 			{ return _actions.end(); }
	/// Gets the number of actions in the timestep.
	inline size_t numActions()							{ return _actions.size(); }

	/// Gets an iterator for the beginning of the list of dynamnic abnormalities occurring in the transition.
	inline const_iterator beginExtActions() const 		{ return _extActions.begin(); }
	/// Gets an iterator for the beginning of the list of dynamic abnormalities occurring in the transition.
	inline const_iterator endExtActions() const 		{ return _extActions.end(); }
	/// Gets the number of dynamic abnormalities in the timestep.
	inline size_t numExtActions() const					{ return _extActions.size(); }	

	/**
	 * Outputs the contents of this timestep to one or more output streams.
	 * @param out - The stream to output to.
	 * @return The number of predicates output in this timestep.
	 */
	size_t output( std::ostream& out ) const;

private:

	/// Simple method used to insert items into a sorted list.
	/// @param list The list to insert the item into.
	/// @param pred The item to insert.
	void insert(PredList& list, Predicate* pred);

	/**
	 * Prints out the provided list and associated label (if anything would print) according to configuration policy.
	 * @param out The output stream to print to.
	 * @param list The list of predicates to print.
	 * @param lbl The label to print out (only prints if any of the predicates in the list would print according to the configuration.
	 * return The number of predicates printed.
	 */
	size_t print(std::ostream& out, PredList const& list, char const* lbl = NULL) const;

};

/// Prints the step to the provided output strea
inline std::ostream& operator<<(std::ostream& lhs, Timestep const& rhs) { rhs.output(lhs); return lhs; }

}; /* end namespace as2transition */

