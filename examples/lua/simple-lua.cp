:- begin_lua.

def a():
	return 7


:- end_lua.


:- constants
	p(computed)	:: sdFluent;
	q :: sdFluent.

caused p(@a()).

caused q where @a() = 7.

