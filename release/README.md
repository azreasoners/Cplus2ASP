# Release Notes Cplus2ASP v3.1

## Translator Limitations
Generally, Cplus2ASP.bin supports the same syntax as CCalc, including many syntactic shortcuts. However, the following features and syntax are not yet supported:
* -togr
* --to-grounder
* -justgr
* --just-grounder
* -grout=
* --grounder-output=
* -gropt=
* --grounder-opts=
* -gr=
* --grounder=
* -gr
* --grounder

## Additional Features
Some capabilities have been added to Cplus2ASP that were not present in the original CCalc. These features may be in the experimental stage, and thus may have their functionality altered or improved as new versions are released.

* Raw Python code: Cplus2ASP supports the insertion of raw Python code in its input files. This allows for greater control and flexibility over the logic of the program. Note that you can use various python libraries in the logic of your code if those libraries are already installed on your machine using python import. See example random-python.cp for such a usage. The directive ":- begin_python." tells Cplus2ASP to start treating any input following it as raw Python code that should be passed through as-is into the translated output. The directive ":- end_python." tells Cplus2ASP to resume processing of an input file as CCalc-style code.
* Config file: Cplus2ASP recognizes options directly from a configuration file. The configuration file can be created in your home directoy and ahs to be named .cplus2asp.config. Note that any argument given on the command line while execution would override the configuration read from the configuration file.

# Release Notes Cplus2ASP v3
## Additional Features
Some capabilities have been added to Cplus2ASP that were not present in the original CCalc. These features may be in the experimental stage, and thus may have their functionality altered or improved as new versions are released.
* Raw LUA code: Cplus2ASP supports the insertion of raw LUA code in its input files. This allows for greater control and flexibility over the logic of the program. The directive ":- begin_lua." tells Cplus2ASP to start treating any input following it as raw LUA code that should be passed through as-is into the translated output. The directive ":- end_lua." tells Cplus2ASP to resume processing of an input file as CCalc-style code.

# Release Notes Cplus2ASP v2
The Cplus2ASP system attempts to support or emulate many features of the original CCalc system. However, Cplus2ASP (in particular, the Cplus2ASP.bin translator module) is currently in the prototype stage of development, and as such certain advanced features of CCalc are not present or not supported in the current version of Cplus2ASP.
## Translator Limitations
Generally, Cplus2ASP.bin supports the same syntax as CCalc, including many syntactic shortcuts. However, the following features and syntax are not yet supported:
* Use of "where" clauses in macro definitions: Simple macro definitions are supported, but "where" clauses are currently ignored.
* External predicates in causal law "where" clauses: "where" clauses are allowed for most causal law forms, but are not evaluated during translation, and are not allowed to contain externally-defined predicates or arbitrary code. Simple uses of causal law "where" clauses, like mathematical evaluation, are allowed.
* Use of min(X,Y) and max(X,Y): CCalc allows these to be used in causal laws and automatically replaces them with the appropriate value X or Y. Cplus2ASP does not directly support this. Workaround: If you need to use one of these operators in a causal law, replace "min(X,Y)" with the expression "V & (X < Y ->> V=X) & (Y =< X ->> V=Y)", where V is an appropriate variable ranging over the values of X and Y. The same is true for "max(X,Y)", just replace the instances of "<" with ">".
## Additional Features
Some capabilities have been added to Cplus2ASP that were not present in the original CCalc. These features may be in the experimental stage, and thus may have their functionality altered or improved as new versions are released.
* Raw ASP/F2LP code: Cplus2ASP supports the insertion of raw ASP/F2LP code in its input files. This allows for more low-level interaction with the translated input, or permits you to create additional ASP-style rules alongside the CCalc-style laws. The directive ":- begin_asp." tells Cplus2ASP to start treating any input following it as raw ASP/F2LP code that should be passed through as-is into the translated output. The directive ":- end_asp." tells Cplus2ASP to resume processing of an input file as CCalc-style code.
## Known Bugs / Deficiencies
There are currently several outstanding bugs in the Cplus2ASP system that will be addressed in new releases in the near future, these are detailed below.
* Errors in macro expansions may occur in certain circumstances while using macros that accept arguments (i.e. foo(#1,#2)).
Workaround: For now these macros should be expanded manually in the event they appear to be expanding improperly.
* Include directives (':- include') can fail when the include lists multiple files. Workaround: Use a single include directive per file, utilizing multiple such directives when necessary.
