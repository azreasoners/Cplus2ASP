:- begin_lua.

function even(num)
return 1 - num % 2
end

:- end_lua.


:- constants
	n			:: simpleFluent(1..10);
	even		:: sdFluent(computed).

caused even=@even(n).
