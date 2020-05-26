#pragma once

#include "bcplus/languages/Language.h"

namespace bcplus {
namespace languages {

/// MVPF language
class MVPF : public Language {

public:
	static const size_t ID;



	virtual char const* name() const;
	virtual bool support(Feature::type feature) const;
	virtual size_t id() const;

};

}}
