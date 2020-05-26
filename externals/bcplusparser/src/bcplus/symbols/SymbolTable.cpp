
#include <boost/version.hpp>
#include <boost/foreach.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/exceptions.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "babb/utils/memory.h"

#include "bcplus/Configuration.h"
#include "bcplus/symbols/Symbol.h"
#include "bcplus/symbols/ConstantSymbol.h"
#include "bcplus/symbols/AttributeSymbol.h"
#include "bcplus/symbols/ObjectSymbol.h"
#include "bcplus/symbols/VariableSymbol.h"
#include "bcplus/symbols/SortSymbol.h"
#include "bcplus/symbols/MacroSymbol.h"
#include "bcplus/symbols/QuerySymbol.h"
#include "bcplus/symbols/SymbolTable.h"
#include "bcplus/symbols/NumberRangeSymbol.h"

#include "bcplus/elements/terms.h"

namespace u = babb::utils;
namespace fs = boost::filesystem;
namespace pt = boost::property_tree;

namespace bcplus {
namespace symbols{

SymbolTable::SymbolTable(Configuration const* config, SymbolMetadataInitializer const* metaInit)
	: _config(config), _metaInit(metaInit), _cmask(0) {
	_good = true;

	// setup the maps

	size_t type = 0x0001;
	while (type <= Symbol::Type::_LARGEST_) {
		_symbols[(Symbol::Type::type)type];
		type = type << 1;
	}

	
	// See if we should load anything from a file
	if (config->symtabInput()) _good = load(*(config->symtabInput()));
	_good = _good && loadMacros(config);

	_bsorts[BuiltinSort::ADDITIVE] 	= resolveOrCreate(new SortSymbol(new ReferencedString("afValue")));
	_bsorts[BuiltinSort::BOOLEAN] 	= resolveOrCreate(new SortSymbol(new ReferencedString("boolean")));
	_bsorts[BuiltinSort::COMPUTED] 	= resolveOrCreate(new SortSymbol(new ReferencedString("computed")));



	// setup the boolean sort
	_bobjs[BuiltinObject::TRUE] 	= resolveOrCreate(new ObjectSymbol(new ReferencedString("true")));
	_bobjs[BuiltinObject::FALSE] 	= resolveOrCreate(new ObjectSymbol(new ReferencedString("false")));
	_bobjs[BuiltinObject::NONE] 	= resolveOrCreate(new ObjectSymbol(new ReferencedString("none")));
	_bobjs[BuiltinObject::UNKNOWN] 	= resolveOrCreate(new ObjectSymbol(new ReferencedString("unknown")));
	

	for (size_t i = 0; i < BuiltinSort::_LENGTH_; i++) {
		if (!_bsorts[i]) _good = false;
	}
	
	for (size_t i = 0; i < BuiltinObject::_LENGTH_; i++) {
		if (!_bobjs[i]) _good = false;
	}




	if (_good) {
		_bsorts[BuiltinSort::ADDITIVE]->add(resolveOrCreate(
			new NumberRangeSymbol(
				new ReferencedString("_afValue_range"),
				new elements::UnaryTerm(elements::UnaryTerm::Operator::NEGATIVE,
					new elements::NullaryTerm(elements::NullaryTerm::Operator::MAXADDITIVE)),
				new elements::NullaryTerm(elements::NullaryTerm::Operator::MAXADDITIVE))));
		_bsorts[BuiltinSort::BOOLEAN]->add(_bobjs[BuiltinObject::TRUE]);
		_bsorts[BuiltinSort::BOOLEAN]->add(_bobjs[BuiltinObject::FALSE]);
	} else {
		config->ostream(Verb::ERROR) << "ERROR: INTERNAL ERROR: An error occurred initializing the builtin constants in the symbol table." << std::endl;
	}
}



SymbolTable::~SymbolTable() {
	// See if we should write anything to a file
	if (_config->symtabOutput() && good()) save(*(_config->symtabOutput()));
}


ReferencedString const* SymbolTable::getData(std::string const& key) const {
	DataMap::const_iterator it = _metadata.find(key);
	if (it == _metadata.end()) return NULL;
	else return it->second;
}

bool SymbolTable::setData(std::string const& key, ReferencedString const* data, bool ovrd) {
	u::ref_ptr<const ReferencedString> data_ptr = data;
	if (!ovrd && getData(key)) return false;
	_metadata[key] = data;
	return true;	
}


Symbol* SymbolTable::resolve(size_t typemask, std::string const& name, size_t arity) {
	return _resolve(typemask, name, arity, true); 
}

Symbol const* SymbolTable::resolve(size_t typemask, std::string const& name, size_t arity) const {
	return _resolve(typemask, name, arity, true);
}


bool SymbolTable::create(Symbol* symbol) {

	// check for uniqueness
	// sorts, queries, and ranges are distinguished from other symbols
	_config->ostream(Verb::TRACE_SYMTAB) << "TRACE: Creating \"" << *symbol->name() << "\" of type \"" << symbol->typeString() << "\"... ";

	size_t mask;
	switch (symbol->type()) {
	case Symbol::Type::SORT:
	case Symbol::Type::RANGE:
		mask = Symbol::Type::SORT | Symbol::Type::RANGE;
		break;
	case Symbol::Type::QUERY:
		mask = Symbol::Type::QUERY;
		break;
	default:
		mask = ~(Symbol::Type::SORT | Symbol::Type::QUERY | Symbol::Type::RANGE);
		break;
	}

	if (_resolve(mask, *(symbol->base()), symbol->arity(), false)) {
		_config->ostream(Verb::TRACE_SYMTAB) << "Failed." << std::endl;
		return false;
	} 

	// Add the symbol
	_symbols[symbol->type()][*(symbol->name())] = symbol;
	_config->ostream(Verb::TRACE_SYMTAB) << "Success!" << std::endl;

	// If this is a constant, make sure to log the type in our mask...
	if (symbol->type() == Symbol::Type::CONSTANT) {
		_cmask |= ((ConstantSymbol const*)symbol)->constType();
	}


	// initialize the metadata
	if (_metaInit) {
		_metaInit->initMetadata(symbol);
	}


	return true;
}

SortSymbol* SymbolTable::carrot(SortSymbol* parent) {
	return plus(parent, bobj(BuiltinObject::UNKNOWN));
}

SortSymbol* SymbolTable::star(SortSymbol* parent) {
	return plus(parent, bobj(BuiltinObject::NONE));
}

SortSymbol* SymbolTable::plus(SortSymbol* parent, ObjectSymbol const* obj) {
	u::ref_ptr<SortSymbol> ret = parent;
	if (!parent->contains(obj)) {
		// make a new  sort symbol symbol...
		ret = resolveOrCreate(new SortSymbol(new ReferencedString(*parent->base() + "__" + *obj->base())));
		ret->addSubSort(parent);
		ret->add(obj);
	}
	return ret;
}

Symbol* SymbolTable::_resolveOrCreate(Symbol* symbol) {
	// reference pointer in order to ensure symbol is properly deallocated if they didn't save a reference to it
	u::ref_ptr<Symbol> symbol_ptr = symbol;

	_config->ostream(Verb::TRACE_SYMTAB) << "TRACE: Resolving \"" << *symbol->name() << "\" of type \"" << symbol->typeString() << "\"... ";
	
	// sorts and queries are distinguished from other symbols
	size_t mask;
	switch (symbol->type()) {
	case Symbol::Type::SORT:
	case Symbol::Type::RANGE:
		mask = Symbol::Type::SORT | Symbol::Type::RANGE;
		break;
	case Symbol::Type::QUERY:
		mask = Symbol::Type::QUERY;
		break;
	default:
		mask = ~(Symbol::Type::SORT | Symbol::Type::QUERY | Symbol::Type::RANGE);
		break;
	}

	// check if the symbol exists
	Symbol* s;

	if ((s = _resolve(mask, *(symbol->base()), symbol->arity(), false))) {
		// the symbol exists, make sure the definition is compatible

		// handle sorts specially as symbol may be a partial specification...
		if (symbol->type() != Symbol::Type::SORT || ((SortSymbol*)symbol)->numSuperSorts() || ((SortSymbol*)symbol)->numSubSorts() || ((SortSymbol*)symbol)->size()) {
			if (*s == *symbol) {
				_config->ostream(Verb::TRACE_SYMTAB) << "Found!" << std::endl;
				return s;
			} else {
				_config->ostream(Verb::TRACE_SYMTAB) << "Found a conflicting identifier definition!" << std::endl;
				return NULL;
			}
		} else {
			// they've provided a sort that has no subsorts, supersorts, or elements.
			// just skip the check and give them what we have.
			_config->ostream(Verb::TRACE_SYMTAB) << "Found!" << std::endl;
			return s;
		}
	} 


	// Add the symbol
	_symbols[symbol->type()][*(symbol->name())] = symbol;
	_config->ostream(Verb::TRACE_SYMTAB) << "Not found.. creating it." << std::endl;


	// If this is a constant, make sure to log the type in our mask...
	if (symbol->type() == Symbol::Type::CONSTANT) {
		_cmask |= ((ConstantSymbol const*)symbol)->constType();
	}

	// initialize the meta data
	if (_metaInit) {
		_metaInit->initMetadata(symbol);
	}

	return symbol;
}

Symbol const* SymbolTable::_resolve(size_t typemask, std::string const& name, size_t arity, bool trace) const {
	std::string s;
	if (strchr(name.c_str(), '/')) {
		s = name;
	} else {
		s = Symbol::genName(name, arity);
	}

	if (trace) _config->ostream(Verb::TRACE_SYMTAB) << "TRACE: Resolving " << s << "... ";

	SymbolMap::const_iterator it;
	size_t type = 0x0001;
	while (type <= Symbol::Type::_LARGEST_) {
		if (type & typemask) {
			TypeMap::const_iterator tit = _symbols.find((Symbol::Type::type)type);
			if (tit != _symbols.end()) {
				SymbolMap const& m = tit->second;
				it = m.find(s);
				if (it != m.end()) {
					if (trace) _config->ostream(Verb::TRACE_SYMTAB) << "Found (" << it->second->typeString() << ")." << std::endl;
					return it->second;
				}
			}
		}
		type = (type << 1);
	} 
	if (trace) _config->ostream(Verb::TRACE_SYMTAB) << "Not found." << std::endl;
	return NULL;
}

Symbol* SymbolTable::_resolve(size_t typemask, std::string const& name, size_t arity, bool trace)  {
	std::string s = Symbol::genName(name, arity);
	if (trace) _config->ostream(Verb::TRACE_SYMTAB) << "TRACE: Resolving \"" << s << "\"... ";

	SymbolMap::iterator it;
	size_t type = 0x0001;
	while (type <= Symbol::Type::_LARGEST_) {
		if (type & typemask) {
			SymbolMap& m = _symbols[(Symbol::Type::type)type];
			it = m.find(s);
			if (it != m.end()) {
				if (trace) _config->ostream(Verb::TRACE_SYMTAB) << "Found (" << it->second->typeString() << ")." << std::endl;
				return it->second;
			}
		}
		type = (type << 1);
	}
	if (trace) _config->ostream(Verb::TRACE_SYMTAB) << "Not found." << std::endl;
	return NULL;
}

bool SymbolTable::load(boost::filesystem::path const& path) {
	fs::ifstream in;
	bool good = true;


	try {
		// make sure the file exists
		if (!fs::exists(path)) {
			_config->ostream(Verb::ERROR) << "ERROR: Could not open file \"" << path.native() << "\". File does not exist." << std::endl;
			return false;
		}
	
		// try to open it
		in.open(path);

		// check if we opened it
		if (in.good()) {
			pt::ptree xml;

			// see about reading the file
			read_xml(in, xml, pt::xml_parser::no_comments | pt::xml_parser::trim_whitespace);

			// scan the file for sorts first then rescan for everything else
			for (int pass = 0; pass < 3; pass++) {
				BOOST_FOREACH(pt::ptree::value_type& symbols, xml) {
					if (boost::iequals(symbols.first, "symbols")) {
						BOOST_FOREACH(pt::ptree::value_type& s, symbols.second) {
							bool add_sym = false;
							u::ref_ptr<Symbol> sym;
							u::ref_ptr<Symbol> tmp;

							switch (Symbol::Type::val(s.first.c_str())) {
							case Symbol::Type::SORT:
								if (pass == 0) {
									// First pass, just create the symbol
									sym = new symbols::SortSymbol(s.second, &(_config->ostream(Verb::ERROR)));
									add_sym = true;
								} else if (pass == 1) {
									// second pass, load the symbol definition
									tmp = new symbols::SortSymbol(s.second, &(_config->ostream(Verb::ERROR)));
									sym = resolveOrCreate(tmp);

									if (!sym) {
										// we weren't able to resolve the sort
										_config->ostream(Verb::ERROR) << "ERROR: Encountered conflicting definitions of sort \"" << *(tmp->base()) << "\" while loading the symbol table from file \"" << path.native() << "\"." << std::endl;
										good = false;
									} else {
										// resolved the sort, try loading the definition
										if (!((symbols::SortSymbol*)sym.get())->loadDefinition(s.second, this, &(_config->ostream(Verb::ERROR)))) {
											_config->ostream(Verb::ERROR) << "ERROR: Encountered conflicting definitions of sort \"" << *(tmp->base()) << "\" while loading the symbol table from file \"" << path.native() << "\"." << std::endl;
											good = false;
										}
									}
								}
								break;
							
							case Symbol::Type::RANGE:
								if (pass == 1) {
									// second pass, create the symbol
									sym = new symbols::NumberRangeSymbol(s.second, this, &(_config->ostream(Verb::ERROR)));
									add_sym = true;
								}
								break;

							case Symbol::Type::CONSTANT:
								if (pass == 1) {
									// second pass, create the symbol
									sym = new symbols::ConstantSymbol(s.second, this, &(_config->ostream(Verb::ERROR)));
									add_sym = true;
								}
								break;
		
							case Symbol::Type::VARIABLE:
								if (pass == 1) {
									// second pass, create the symbol
									sym = new symbols::VariableSymbol(s.second, this, &(_config->ostream(Verb::ERROR)));
									add_sym = true;
								}
								break;

							case Symbol::Type::OBJECT:
								if (pass == 1) {
									// second pass, create the symbol
									sym = new symbols::ObjectSymbol(s.second, this, &(_config->ostream(Verb::ERROR)));
									add_sym = true;
								}
								break;

							case Symbol::Type::MACRO:
								if (pass == 1) {
									sym = new symbols::MacroSymbol(s.second, &(_config->ostream(Verb::ERROR)));
									add_sym = true;
								}
								break;

							case Symbol::Type::QUERY:
								if (pass == 1) {
									sym = new symbols::QuerySymbol(s.second, this, &(_config->ostream(Verb::ERROR)));
									add_sym = true;
								}

								break;

							case Symbol::Type::ERR_INVALID_SYMBOL:

								// check if it's an attribute
								if (boost::iequals(s.first, "attribute")) {
									if (pass == 2) {
										sym = new symbols::AttributeSymbol(s.second, this, &(_config->ostream(Verb::ERROR)));
										add_sym = true; 		
									}
								} else {
									_config->ostream(Verb::ERROR) << "ERROR: Encountered unexpected symbol key \"" << s.first << "\" in symbol table file \"" << path.native() << "\". Expected one of \"sort\", \"constant\", \"variable\", \"object\", or \"macro\"." << std::endl;
									good = false;
								}
							};

							// finish up by adding the symbol if we need to.
							if (add_sym) {
								if (!sym->good()) {
									_config->ostream(Verb::ERROR) << "ERROR: An error occurred while trying to load a symbol of type \"" << s.first << "\" in symbol table file \"" << path.native() << "\"." << std::endl;
									good = false;
								} else {
									// try to create the symbol
									tmp = resolveOrCreate(sym);
									if (!tmp) {
										_config->ostream(Verb::ERROR) << "ERROR: Detected a conflicting definition of symbol \"" << *(sym->name()) << "\" while loading symbol table from file \"" << path.native() << "\"." << std::endl;
										good = false;
									}
								}
							}
						}
					} else if (boost::iequals(symbols.first, "metadata")) {
						if (pass == 0) {
							std::string name, value;
							BOOST_FOREACH(pt::ptree::value_type& s, symbols.second) {
								bool goodentry = true;
								if (!boost::iequals(s.first, "value")) {
									_config->ostream(Verb::ERROR) << "ERROR: Encountered unexpected key \"" << symbols.first << "\" while scanning the metadata information in symbol table file \"" << path.native() << "\". Expected \"value\"." << std::endl;
									good = false;
									continue;
								} 
								try {
									name = s.second.get<std::string>("<xmlattr>.key");
								} catch (boost::property_tree::ptree_error const& e) {
									_config->ostream(Verb::ERROR) << "ERROR: Encountered a malformed metadata entry while scanning symbol table file \"" << path.native() << "\". The entry is missing its \"key\" attribute." << std::endl;
									good = false;
									goodentry = false;
								}
	
								try {
									value = s.second.get<std::string>("<xmlattr>.value");
								} catch (boost::property_tree::ptree_error const& e) {
									if (goodentry)
										_config->ostream(Verb::ERROR) << "ERROR: Encountered a malformed metadata entry while scanning symbol table file \"" << path.native() << "\". The entry is missing its \"value\" attribute." << std::endl;
									else
										_config->ostream(Verb::ERROR) << "ERROR: Metadata entry \"" << name << "\" is malformed in symbol table file \"" << path.native() << "\". The entry is missing its \"value\" attribute." << std::endl;
			
									good = false;
									goodentry = false;
								}
						
								if (goodentry && !setData(name, new ReferencedString(value))) {
									_config->ostream(Verb::ERROR) << "ERROR: Encountered a redefinition of metadata entry \"" << name  << "\" in symbol table file \"" << path.native() << "\"." << std::endl;
									good = false;
								}
							}
						}

					} else {
						_config->ostream(Verb::ERROR) << "ERROR: Encountered unexpected top-level key \"" << symbols.first << "\" in symbol table file \"" << path.native() << "\". Expected \"symbols\" or \"metadata\"." << std::endl;
						good = false;
					}
				}
			}

		} else {
			_config->ostream(Verb::ERROR) << "ERROR: An error occurred while opening file \"" << path.native() << "\"." << std::endl;
			good = false;
		}


	} catch (fs::filesystem_error& e) {
		_config->ostream(Verb::ERROR) << "ERROR: An error occurred while reading from \"" << path.native() << "\"." << std::endl;
		good = false;
	} catch (pt::xml_parser::xml_parser_error& e) {
		_config->ostream(Verb::ERROR) << "ERROR: An error occurred parsing the symbol table input file \"" << path.native() << "\"." << std::endl;
		good = false;
	}

	if (in.is_open()) in.close();	
	if (!good) {
		_config->ostream(Verb::ERROR) << "ERROR: An error occurred while loading the symbol table from file \"" << path.native() << "\"." << std::endl;
	}

	return good;

}

bool SymbolTable::save(boost::filesystem::path const& path) const {

	// create the XML property tree
	pt::ptree xml;
	pt::ptree& symbols = xml.put("symbols", "");

	size_t type = 0x0001;
	while (type <= Symbol::Type::_LARGEST_) {
		char const* typestr = Symbol::Type::cstr((Symbol::Type::type)type);
		for (const_iterator it = begin((Symbol::Type::type)type); it != end((Symbol::Type::type)type); it++) {
			pt::ptree& node = symbols.add(typestr, "");
			if (*it) (*it)->save(node);
		}
		type = type << 1;
	}

	// Also include the metadata
	pt::ptree& metadata = xml.put("metadata", "");
	BOOST_FOREACH(DataMap::value_type const& data, _metadata) {
		pt::ptree& node = metadata.add("value", "");
		node.put("<xmlattr>.key", data.first);
		node.put("<xmlattr>.value", *data.second);
	}



	// open the file
	fs::ofstream out;
	bool good = true;


	try {

		// try to open it
		out.open(path);

		// check if we opened it
		if (!out.good()) {
			_config->ostream(Verb::ERROR) << "ERROR: Unable to open symbol table output file \"" << path.native() << "\"." << std::endl;
			good = false;
		}


		// write the file
#if BOOST_VERSION >= 105600
		pt::xml_parser::write_xml(out, xml, pt::xml_parser::xml_writer_settings<std::string>('\t', 1));
#else
		pt::xml_parser::write_xml(out, xml, pt::xml_parser::xml_writer_settings<char>('\t', 1));
#endif

	} catch (fs::filesystem_error& e) {
		_config->ostream(Verb::ERROR) << "ERROR: An error occurred while writing the symbol table to \"" << path.native() << "\"." << std::endl;
		good = false;
	} catch (pt::xml_parser::xml_parser_error& e) {
		_config->ostream(Verb::ERROR) << "ERROR: An error occurred while writing the symbol table to \"" << path.native() << "\"." << std::endl;
		good = false;	
	}

	if (out.is_open()) out.close();
	return good;
}

bool SymbolTable::loadMacros(Configuration const* config) {
	bool good = true;
	BOOST_FOREACH(Configuration::MacroDefinition const& macro, *config) {
		if (*macro.first == "maxAdditive" || *macro.first == "maxAFValue") {
			// Ensure it's an integer!
			bool goodval = true;
			try {
				boost::lexical_cast<int>(*macro.second);
			} catch (boost::bad_lexical_cast const& e) {
				_config->ostream(Verb::ERROR) << "ERROR: Encountered a bad command-line definition of \"maxAdditive\". \"" + *macro.second + "\" is not a non-negative integer." << std::endl;
				good = false;
				goodval = false;
			}

			if (goodval && !setData(*macro.first, macro.second)) {
				config->ostream(Verb::ERROR) << "ERROR: Detected redeclaration of \"" + *macro.first + "\" while scanning command line macro definitions." << std::endl;
				good = false;
			}
		} else {

			u::ref_ptr<MacroSymbol> m = new MacroSymbol(macro.first, macro.second);
			if (!create(m)) {
				good = false;
				config->ostream(Verb::ERROR) << "ERROR: Detected redeclaration of symbol \"" + *macro.first + "/0\" while scanning command line macro definitions." << std::endl;
			}
		}
	}

	return good;
}


}}
