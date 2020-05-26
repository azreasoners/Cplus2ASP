%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% cplus2asp v. 2.0 Changelog
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% January 2013
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% V2.0 Author: Joseph Babb
% V1.0 Author: Michael Cassolary 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% cplus2asp
% ---------------------------------------------------------------------------------------

* Complete rewrite to improve system reliability and maintainability.

* Removed explicity grounder/solvers from the toolchain. grounder/solvers may still be
  used


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% cplus2asp.bin
% ---------------------------------------------------------------------------------------

* Fixed an issue causing extra atoms added to a rule during translation to be added within
  the wrong context.

  Example:
  caused false if [\/X | p(X)=q(X)].

  Original Translation:
  false <- ?[X]:( X_Value1 = X_Value2 ) 
	& h(eql(p(X),X_Value1),V_step)
	& h(eql(q(X),X_Value2),V_step).

  Correct Translation:
  false <- ?[X]: ( X_Value1 = X_Value2
	    & h(eql(p(X),X_Value1),V_step)
	    & h(eql(q(X),X_Value2),V_step)
	).

* Simplified the standard file for readability.

* Shifted step index from step T and T+1 to step T-1 and T.

* Modified the translation to utilize a separate top level predicate for action constants. 
  These constants now use the 'occ/2' predicate, rather than 'h/2'.

* Added the strong_noconcurrency statement, which, when asserted, provides that 2 
  non-attribute, non-abAction actions may not take a value other than 'false' (boolean) or 
  'none' (multi-valued) simultaneously.

* Added support for the 'assuming' clause in causal laws. This clause behaves identical to
  the 'if' clause except that it is not placed in the scope of double negation.

  Example:
  caused p if p.
  translates to
  h(eql(p,true),V_step) <- not not h(eql(p,true),V_step).
  whereas
  caused p assuming p.
  translates to
  h(eql(p,true),V_step) <- h(eql(p,true),V_step).

* Enhanced the error detection and reporting capabilities. Many static semantic errors
  are now detected and reported to the user. Line numbers are provided a greater amount
  of the time for all errors.

* Added support for the aC+ abnormality constants (staticAbnormality and dynamicAbnormality),
  and the corresponding 'when' and 'following' clauses of causal laws. These clauses allow
  for a rule to be labeled with nested propositional formulas containing only abnormality
  constants. 

  Example
  caused p after a following not ab_a.

  Translates to:
  caused h(eql(p,true),V_astep) <-
	occ(eql(p,true),V_astep-1)
	& not (ab_occ(ab_a,V_astep-1)).

* Added enhanced LUA call handling. External function calls must now begin with the '@' symbol.
  External function calls can now be non-boolean and can be nested within constants, objects,
  and other external function calls. Added checks to ensure that External function calls may 
  only appear in the where clause of a rule.

* Added the '--symbol-table [file]' command line option, which outputs the translator's symbol
  table to a file after translation has finished. 

* Added the '--expressive-return' command line option, which enables several flags in the return
  status. Specifically, the 0x80 bit will be set to indicate that abnormality constants are
  present in the program and the 0x40 bit will be set to indicate that additive constants are
  present in the program.

* Now returns an error code of 1 in the event of an error parsing command line arguments.

* Implemented the ability to detect undeclared identifiers. An error will now be thrown if
  such an identifier is detected.

* Removed the requirement for queries to have a declared maximum step value. If this is required
  (i.e. we're performining a static translation) then the wrapper will handle the deficiency.

* Added the ability to use anonymous sorts specified with a number range while declaring
  constants and objects.

* Added the shortcut rule 'c=v.' which expands to 'caused c=v.'

* Added the 'never' alias for 'impossible' constraints.

* The translator no longer fails in the presence of a 'show' statement. Show statements are
  still not currently supported, however they have been downgraded from an error to a caution.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% as2transition
% ---------------------------------------------------------------------------------------

* It is no longer required to include the action/1, fluent/1, and rigid/1 top level
  predicates in the answer sets in order for as2transition to properly parse them.

* Now supports new top level predicates 'occ/2', 'ab_h/2', and 'ab_occ/2', which are used
  to capture action constants, static abnormality constants, and dynamic abnormality 
  constants, respectively.

* Added additional sections for the output which are used to display static and dynamic
  abnormality predicates which are present within each time step within the answer sets.

* Now supports specifying an input file without the -f flag.

* Now returns an error code of 1 in the event of an error parsing command line arguments.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% F2LP
% ---------------------------------------------------------------------------------------

* Increased the maximum size of a comment to 4096 character.

* Improved the argument handling to allow for interchangeable file / option specification.

* Added the -i flag to indicate that the output should be targetted to i/oClingo. This
  option prevents the addition of 'false' to all constraint heads and ensures that the
  added definitions for 'true.' and ':- false.' occur in the scope of #base.