#pragma once

namespace bcplus {

/// Container of enumeration of description of domain.
struct DomainType {
	enum type {
		INTEGRAL,			///< Ranges over numbers
		BOOLEAN,			///< Ranges over "true" and "false"
		NO_DOMAIN,			///< Does not have an associated domain
		OTHER,				///< Ranges over various objects
		UNKNOWN				///< All bets are off.
	};
};


};
