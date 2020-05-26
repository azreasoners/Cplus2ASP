
:- begin_lua.

function rand()
	math.randomseed(os.time())
	math.random()
	return math.random(0, 100)
end

:- end_lua.

:- sorts
	nums.

:- objects
	0..100	:: nums.

:- constants
	r		:: nums.

:- variables
	R		:: nums.



caused r=R where R = @rand().

