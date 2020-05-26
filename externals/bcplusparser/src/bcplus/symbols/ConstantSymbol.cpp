/* Generated by re2c 0.13.5 on Thu Dec 17 18:02:06 2015 */
#line 1 "bcplus/symbols/ConstantSymbol.r2c"

#include <ostream>

#include "bcplus/DomainType.h"
#include "bcplus/symbols/Resolver.h"
#include "bcplus/symbols/Symbol.h"
#include "bcplus/symbols/detail/BaseSymbol.h"
#include "bcplus/symbols/ConstantSymbol.h"
#include "bcplus/symbols/SortSymbol.h"


namespace bcplus {
namespace symbols {

char const* ConstantSymbol::Type::cstr(type val) {
	switch (val) {
		case ABACTION:					return "abAction";
		case ACTION:					return "action";
		case ADDITIVEFLUENT:			return "additiveFluent";
		case ADDITIVEACTION:			return "additiveAction";
		case ATTRIBUTE:					return "attribute";
		case EXTERNALACTION:			return "externalAction";
		case EXTERNALFLUENT:			return "externalFluent";
		case EXOGENOUSACTION:			return "exogenousAction";
		case INERTIALFLUENT:			return "inertialFluent";
		case RIGID:						return "rigid";
		case SDFLUENT:					return "sdFluent";
		case SIMPLEFLUENT:				return "simpleFluent";
		default:						return "<unknown_type>";
	}

}

ConstantSymbol::Type::type ConstantSymbol::Type::value(char const* str) {
	char const* marker;
	
#line 40 "<stdout>"
	{
		char yych;

		yych = *str;
		switch (yych) {
		case '\n':	goto yy2;
		case 'A':
		case 'a':	goto yy3;
		case 'E':
		case 'e':	goto yy5;
		case 'I':
		case 'i':	goto yy6;
		case 'R':
		case 'r':	goto yy7;
		case 'S':
		case 's':	goto yy8;
		default:	goto yy9;
		}
yy2:
		str = marker;
		goto yy4;
yy3:
		yych = *(marker = ++str);
		switch (yych) {
		case 'B':
		case 'b':	goto yy87;
		case 'C':
		case 'c':	goto yy86;
		case 'D':
		case 'd':	goto yy85;
		case 'T':
		case 't':	goto yy84;
		default:	goto yy4;
		}
yy4:
#line 55 "bcplus/symbols/ConstantSymbol.r2c"
		{ return ERR_UNKNOWN; }
#line 78 "<stdout>"
yy5:
		yych = *(marker = ++str);
		switch (yych) {
		case 'X':
		case 'x':	goto yy49;
		default:	goto yy4;
		}
yy6:
		yych = *(marker = ++str);
		switch (yych) {
		case 'N':
		case 'n':	goto yy35;
		default:	goto yy4;
		}
yy7:
		yych = *(marker = ++str);
		switch (yych) {
		case 'I':
		case 'i':	goto yy30;
		default:	goto yy4;
		}
yy8:
		yych = *(marker = ++str);
		switch (yych) {
		case 'D':
		case 'd':	goto yy11;
		case 'I':
		case 'i':	goto yy10;
		default:	goto yy4;
		}
yy9:
		yych = *++str;
		goto yy4;
yy10:
		yych = *++str;
		switch (yych) {
		case 'M':
		case 'm':	goto yy19;
		default:	goto yy2;
		}
yy11:
		yych = *++str;
		switch (yych) {
		case 'F':
		case 'f':	goto yy12;
		default:	goto yy2;
		}
yy12:
		yych = *++str;
		switch (yych) {
		case 'L':
		case 'l':	goto yy13;
		default:	goto yy2;
		}
yy13:
		yych = *++str;
		switch (yych) {
		case 'U':
		case 'u':	goto yy14;
		default:	goto yy2;
		}
yy14:
		yych = *++str;
		switch (yych) {
		case 'E':
		case 'e':	goto yy15;
		default:	goto yy2;
		}
yy15:
		yych = *++str;
		switch (yych) {
		case 'N':
		case 'n':	goto yy16;
		default:	goto yy2;
		}
yy16:
		yych = *++str;
		switch (yych) {
		case 'T':
		case 't':	goto yy17;
		default:	goto yy2;
		}
yy17:
		++str;
#line 53 "bcplus/symbols/ConstantSymbol.r2c"
		{ return SDFLUENT; }
#line 165 "<stdout>"
yy19:
		yych = *++str;
		switch (yych) {
		case 'P':
		case 'p':	goto yy20;
		default:	goto yy2;
		}
yy20:
		yych = *++str;
		switch (yych) {
		case 'L':
		case 'l':	goto yy21;
		default:	goto yy2;
		}
yy21:
		yych = *++str;
		switch (yych) {
		case 'E':
		case 'e':	goto yy22;
		default:	goto yy2;
		}
yy22:
		yych = *++str;
		switch (yych) {
		case 'F':
		case 'f':	goto yy23;
		default:	goto yy2;
		}
yy23:
		yych = *++str;
		switch (yych) {
		case 'L':
		case 'l':	goto yy24;
		default:	goto yy2;
		}
yy24:
		yych = *++str;
		switch (yych) {
		case 'U':
		case 'u':	goto yy25;
		default:	goto yy2;
		}
yy25:
		yych = *++str;
		switch (yych) {
		case 'E':
		case 'e':	goto yy26;
		default:	goto yy2;
		}
yy26:
		yych = *++str;
		switch (yych) {
		case 'N':
		case 'n':	goto yy27;
		default:	goto yy2;
		}
yy27:
		yych = *++str;
		switch (yych) {
		case 'T':
		case 't':	goto yy28;
		default:	goto yy2;
		}
yy28:
		++str;
#line 54 "bcplus/symbols/ConstantSymbol.r2c"
		{ return SIMPLEFLUENT; }
#line 233 "<stdout>"
yy30:
		yych = *++str;
		switch (yych) {
		case 'G':
		case 'g':	goto yy31;
		default:	goto yy2;
		}
yy31:
		yych = *++str;
		switch (yych) {
		case 'I':
		case 'i':	goto yy32;
		default:	goto yy2;
		}
yy32:
		yych = *++str;
		switch (yych) {
		case 'D':
		case 'd':	goto yy33;
		default:	goto yy2;
		}
yy33:
		++str;
#line 52 "bcplus/symbols/ConstantSymbol.r2c"
		{ return RIGID; }
#line 259 "<stdout>"
yy35:
		yych = *++str;
		switch (yych) {
		case 'E':
		case 'e':	goto yy36;
		default:	goto yy2;
		}
yy36:
		yych = *++str;
		switch (yych) {
		case 'R':
		case 'r':	goto yy37;
		default:	goto yy2;
		}
yy37:
		yych = *++str;
		switch (yych) {
		case 'T':
		case 't':	goto yy38;
		default:	goto yy2;
		}
yy38:
		yych = *++str;
		switch (yych) {
		case 'I':
		case 'i':	goto yy39;
		default:	goto yy2;
		}
yy39:
		yych = *++str;
		switch (yych) {
		case 'A':
		case 'a':	goto yy40;
		default:	goto yy2;
		}
yy40:
		yych = *++str;
		switch (yych) {
		case 'L':
		case 'l':	goto yy41;
		default:	goto yy2;
		}
yy41:
		yych = *++str;
		switch (yych) {
		case 'F':
		case 'f':	goto yy42;
		default:	goto yy2;
		}
yy42:
		yych = *++str;
		switch (yych) {
		case 'L':
		case 'l':	goto yy43;
		default:	goto yy2;
		}
yy43:
		yych = *++str;
		switch (yych) {
		case 'U':
		case 'u':	goto yy44;
		default:	goto yy2;
		}
yy44:
		yych = *++str;
		switch (yych) {
		case 'E':
		case 'e':	goto yy45;
		default:	goto yy2;
		}
yy45:
		yych = *++str;
		switch (yych) {
		case 'N':
		case 'n':	goto yy46;
		default:	goto yy2;
		}
yy46:
		yych = *++str;
		switch (yych) {
		case 'T':
		case 't':	goto yy47;
		default:	goto yy2;
		}
yy47:
		++str;
#line 51 "bcplus/symbols/ConstantSymbol.r2c"
		{ return INERTIALFLUENT; }
#line 348 "<stdout>"
yy49:
		yych = *++str;
		switch (yych) {
		case 'O':
		case 'o':	goto yy51;
		case 'T':
		case 't':	goto yy50;
		default:	goto yy2;
		}
yy50:
		yych = *++str;
		switch (yych) {
		case 'E':
		case 'e':	goto yy65;
		default:	goto yy2;
		}
yy51:
		yych = *++str;
		switch (yych) {
		case 'G':
		case 'g':	goto yy52;
		default:	goto yy2;
		}
yy52:
		yych = *++str;
		switch (yych) {
		case 'E':
		case 'e':	goto yy53;
		default:	goto yy2;
		}
yy53:
		yych = *++str;
		switch (yych) {
		case 'N':
		case 'n':	goto yy54;
		default:	goto yy2;
		}
yy54:
		yych = *++str;
		switch (yych) {
		case 'O':
		case 'o':	goto yy55;
		default:	goto yy2;
		}
yy55:
		yych = *++str;
		switch (yych) {
		case 'U':
		case 'u':	goto yy56;
		default:	goto yy2;
		}
yy56:
		yych = *++str;
		switch (yych) {
		case 'S':
		case 's':	goto yy57;
		default:	goto yy2;
		}
yy57:
		yych = *++str;
		switch (yych) {
		case 'A':
		case 'a':	goto yy58;
		default:	goto yy2;
		}
yy58:
		yych = *++str;
		switch (yych) {
		case 'C':
		case 'c':	goto yy59;
		default:	goto yy2;
		}
yy59:
		yych = *++str;
		switch (yych) {
		case 'T':
		case 't':	goto yy60;
		default:	goto yy2;
		}
yy60:
		yych = *++str;
		switch (yych) {
		case 'I':
		case 'i':	goto yy61;
		default:	goto yy2;
		}
yy61:
		yych = *++str;
		switch (yych) {
		case 'O':
		case 'o':	goto yy62;
		default:	goto yy2;
		}
yy62:
		yych = *++str;
		switch (yych) {
		case 'N':
		case 'n':	goto yy63;
		default:	goto yy2;
		}
yy63:
		++str;
#line 50 "bcplus/symbols/ConstantSymbol.r2c"
		{ return EXOGENOUSACTION; }
#line 453 "<stdout>"
yy65:
		yych = *++str;
		switch (yych) {
		case 'R':
		case 'r':	goto yy66;
		default:	goto yy2;
		}
yy66:
		yych = *++str;
		switch (yych) {
		case 'N':
		case 'n':	goto yy67;
		default:	goto yy2;
		}
yy67:
		yych = *++str;
		switch (yych) {
		case 'A':
		case 'a':	goto yy68;
		default:	goto yy2;
		}
yy68:
		yych = *++str;
		switch (yych) {
		case 'L':
		case 'l':	goto yy69;
		default:	goto yy2;
		}
yy69:
		yych = *++str;
		switch (yych) {
		case 'A':
		case 'a':	goto yy70;
		case 'F':
		case 'f':	goto yy71;
		default:	goto yy2;
		}
yy70:
		yych = *++str;
		switch (yych) {
		case 'C':
		case 'c':	goto yy78;
		default:	goto yy2;
		}
yy71:
		yych = *++str;
		switch (yych) {
		case 'L':
		case 'l':	goto yy72;
		default:	goto yy2;
		}
yy72:
		yych = *++str;
		switch (yych) {
		case 'U':
		case 'u':	goto yy73;
		default:	goto yy2;
		}
yy73:
		yych = *++str;
		switch (yych) {
		case 'E':
		case 'e':	goto yy74;
		default:	goto yy2;
		}
yy74:
		yych = *++str;
		switch (yych) {
		case 'N':
		case 'n':	goto yy75;
		default:	goto yy2;
		}
yy75:
		yych = *++str;
		switch (yych) {
		case 'T':
		case 't':	goto yy76;
		default:	goto yy2;
		}
yy76:
		++str;
#line 49 "bcplus/symbols/ConstantSymbol.r2c"
		{ return EXTERNALFLUENT; }
#line 537 "<stdout>"
yy78:
		yych = *++str;
		switch (yych) {
		case 'T':
		case 't':	goto yy79;
		default:	goto yy2;
		}
yy79:
		yych = *++str;
		switch (yych) {
		case 'I':
		case 'i':	goto yy80;
		default:	goto yy2;
		}
yy80:
		yych = *++str;
		switch (yych) {
		case 'O':
		case 'o':	goto yy81;
		default:	goto yy2;
		}
yy81:
		yych = *++str;
		switch (yych) {
		case 'N':
		case 'n':	goto yy82;
		default:	goto yy2;
		}
yy82:
		++str;
#line 48 "bcplus/symbols/ConstantSymbol.r2c"
		{ return EXTERNALACTION; }
#line 570 "<stdout>"
yy84:
		yych = *++str;
		switch (yych) {
		case 'T':
		case 't':	goto yy120;
		default:	goto yy2;
		}
yy85:
		yych = *++str;
		switch (yych) {
		case 'D':
		case 'd':	goto yy100;
		default:	goto yy2;
		}
yy86:
		yych = *++str;
		switch (yych) {
		case 'T':
		case 't':	goto yy95;
		default:	goto yy2;
		}
yy87:
		yych = *++str;
		switch (yych) {
		case 'A':
		case 'a':	goto yy88;
		default:	goto yy2;
		}
yy88:
		yych = *++str;
		switch (yych) {
		case 'C':
		case 'c':	goto yy89;
		default:	goto yy2;
		}
yy89:
		yych = *++str;
		switch (yych) {
		case 'T':
		case 't':	goto yy90;
		default:	goto yy2;
		}
yy90:
		yych = *++str;
		switch (yych) {
		case 'I':
		case 'i':	goto yy91;
		default:	goto yy2;
		}
yy91:
		yych = *++str;
		switch (yych) {
		case 'O':
		case 'o':	goto yy92;
		default:	goto yy2;
		}
yy92:
		yych = *++str;
		switch (yych) {
		case 'N':
		case 'n':	goto yy93;
		default:	goto yy2;
		}
yy93:
		++str;
#line 43 "bcplus/symbols/ConstantSymbol.r2c"
		{ return ABACTION; }
#line 638 "<stdout>"
yy95:
		yych = *++str;
		switch (yych) {
		case 'I':
		case 'i':	goto yy96;
		default:	goto yy2;
		}
yy96:
		yych = *++str;
		switch (yych) {
		case 'O':
		case 'o':	goto yy97;
		default:	goto yy2;
		}
yy97:
		yych = *++str;
		switch (yych) {
		case 'N':
		case 'n':	goto yy98;
		default:	goto yy2;
		}
yy98:
		++str;
#line 44 "bcplus/symbols/ConstantSymbol.r2c"
		{ return ACTION; }
#line 664 "<stdout>"
yy100:
		yych = *++str;
		switch (yych) {
		case 'I':
		case 'i':	goto yy101;
		default:	goto yy2;
		}
yy101:
		yych = *++str;
		switch (yych) {
		case 'T':
		case 't':	goto yy102;
		default:	goto yy2;
		}
yy102:
		yych = *++str;
		switch (yych) {
		case 'I':
		case 'i':	goto yy103;
		default:	goto yy2;
		}
yy103:
		yych = *++str;
		switch (yych) {
		case 'V':
		case 'v':	goto yy104;
		default:	goto yy2;
		}
yy104:
		yych = *++str;
		switch (yych) {
		case 'E':
		case 'e':	goto yy105;
		default:	goto yy2;
		}
yy105:
		yych = *++str;
		switch (yych) {
		case 'A':
		case 'a':	goto yy107;
		case 'F':
		case 'f':	goto yy106;
		default:	goto yy2;
		}
yy106:
		yych = *++str;
		switch (yych) {
		case 'L':
		case 'l':	goto yy114;
		default:	goto yy2;
		}
yy107:
		yych = *++str;
		switch (yych) {
		case 'C':
		case 'c':	goto yy108;
		default:	goto yy2;
		}
yy108:
		yych = *++str;
		switch (yych) {
		case 'T':
		case 't':	goto yy109;
		default:	goto yy2;
		}
yy109:
		yych = *++str;
		switch (yych) {
		case 'I':
		case 'i':	goto yy110;
		default:	goto yy2;
		}
yy110:
		yych = *++str;
		switch (yych) {
		case 'O':
		case 'o':	goto yy111;
		default:	goto yy2;
		}
yy111:
		yych = *++str;
		switch (yych) {
		case 'N':
		case 'n':	goto yy112;
		default:	goto yy2;
		}
yy112:
		++str;
#line 46 "bcplus/symbols/ConstantSymbol.r2c"
		{ return ADDITIVEACTION; }
#line 755 "<stdout>"
yy114:
		yych = *++str;
		switch (yych) {
		case 'U':
		case 'u':	goto yy115;
		default:	goto yy2;
		}
yy115:
		yych = *++str;
		switch (yych) {
		case 'E':
		case 'e':	goto yy116;
		default:	goto yy2;
		}
yy116:
		yych = *++str;
		switch (yych) {
		case 'N':
		case 'n':	goto yy117;
		default:	goto yy2;
		}
yy117:
		yych = *++str;
		switch (yych) {
		case 'T':
		case 't':	goto yy118;
		default:	goto yy2;
		}
yy118:
		++str;
#line 45 "bcplus/symbols/ConstantSymbol.r2c"
		{ return ADDITIVEFLUENT; }
#line 788 "<stdout>"
yy120:
		yych = *++str;
		switch (yych) {
		case 'R':
		case 'r':	goto yy121;
		default:	goto yy2;
		}
yy121:
		yych = *++str;
		switch (yych) {
		case 'I':
		case 'i':	goto yy122;
		default:	goto yy2;
		}
yy122:
		yych = *++str;
		switch (yych) {
		case 'B':
		case 'b':	goto yy123;
		default:	goto yy2;
		}
yy123:
		yych = *++str;
		switch (yych) {
		case 'U':
		case 'u':	goto yy124;
		default:	goto yy2;
		}
yy124:
		yych = *++str;
		switch (yych) {
		case 'T':
		case 't':	goto yy125;
		default:	goto yy2;
		}
yy125:
		yych = *++str;
		switch (yych) {
		case 'E':
		case 'e':	goto yy126;
		default:	goto yy2;
		}
yy126:
		++str;
#line 47 "bcplus/symbols/ConstantSymbol.r2c"
		{ return ATTRIBUTE; }
#line 835 "<stdout>"
	}
#line 57 "bcplus/symbols/ConstantSymbol.r2c"


}



ConstantSymbol::ConstantSymbol(Type::type type, ReferencedString const* base, SortSymbol const* sort, SortList const* args) 
	: BaseSymbol(Symbol::Type::CONSTANT, base, args) , _sort(sort), _type(type) {
	// intentionally left blank
}

ConstantSymbol::ConstantSymbol(boost::property_tree::ptree const& node, Resolver const* resolver, std::ostream* err)
	: BaseSymbol(Symbol::Type::CONSTANT, node, resolver, err) {

	try {
		std::string sort_name = node.get<std::string>("<xmlattr>.sort");
		SortSymbol const* sort = (SortSymbol const*)resolver->resolve(Symbol::Type::SORT, sort_name);
		if (!sort) {
			good(false);
			if (err) *err << "ERROR: An error occurred while scanning the definition of \"" << *name() << "\". \"" << sort_name << "\" is not a valid sort." << std::endl;
		} else {
			_sort = sort;
		}



	} catch (boost::property_tree::ptree_error& e) {
		good(false);
		if (err) *err << "ERROR: An error occurred while scanning the definition of \"" << *name() << "\". Expected a 'sort' attribute declaring the constant's sort." << std::endl;
	}

	try {

		std::string type_name = node.get<std::string>("<xmlattr>.type");
		Type::type t = Type::value(type_name.c_str());

		if (t == Type::ERR_UNKNOWN) {
			good(false);
			if (err) *err << "ERROR: An error occurred while scanning the definition of \"" << *name() << "\". Encoutered an unknown type specifier \"" << type_name << "\"." << std::endl;

		} else _type = t;


	} catch (boost::property_tree::ptree_error& e) {
		good(false);
		if (err) *err << "ERROR: An error occurred while scanning the definition of \"" << *name() << "\". Expected a 'type' attribute declaring the constant's type." << std::endl;
	}


}

ConstantSymbol::~ConstantSymbol() {
	// Intentionally left blank
}


DomainType::type ConstantSymbol::domainType() const {
	return _sort->domainType();
}

bool ConstantSymbol::operator==(Symbol const& other) const {
	if (!BaseSymbol::operator==(other)) return false;
	ConstantSymbol const& o = (ConstantSymbol const&)other;

	// ensure the sort is the same
	if (sort() != o.sort()) return false;
	return true;
}


void ConstantSymbol::save(boost::property_tree::ptree& node) const {
	BaseSymbol::save(node);
	node.put("<xmlattr>.sort", *_sort->base());
	node.put("<xmlattr>.type", constTypeString());
}

}}
