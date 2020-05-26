#pragma once

#include "bcplus/languages/Language.h"

namespace bcplus {
namespace languages {

/// BC+ language
class OBCPlus : public Language {

public:
	static const size_t ID;



	virtual char const* name() const;
	virtual bool support(Feature::type feature) const;
	virtual size_t id() const;

};

}}
