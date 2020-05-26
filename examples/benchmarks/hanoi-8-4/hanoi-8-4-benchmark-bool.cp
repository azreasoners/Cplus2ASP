% file 'hanoi-8-4-benchmark-bool.cp'.
% Towers of Hanoi

:- sorts
	location >> disc; 
	location >> peg;
	int.

:- macros
	M -> 8;
	N -> 4.

:- objects
	1..M :: int;
	disc(int) :: disc;
	peg(1..N) :: peg.

:- constants
	on(disc, location) :: simpleFluent;
	move(disc,location) :: exogenousAction.

:- variables
	D, D1 :: disc;
	P, P1 :: peg;
	L, L1 :: location;
	I, I1 :: int.


inertial on(D,L).

% after moving disc it is at new place
caused on(D, L) after move(D, L).

% disc is only on top of one other disc
caused -on(D, L) if on(D, L1) where L \= L1.


% no concurrency
nonexecutable move(D, L) & move(D1, L1) where D \= D1.
nonexecutable move(D, L) & move(D1, L1) where L \= L1.


% only move disc to location that doesn't have any other disc on top
nonexecutable move(D, L) if on(D1, L) where D \= D1.

% only move disc when it is the topmost disc
nonexecutable move(D, L) if on(D1, D) where D \= D1.

% only move disc on top of bigger disc
nonexecutable move(disc(I), disc(I1)) where I >= I1.

:- query
	label::0;
	maxstep:: 0..100;
	0: on(disc(M), peg(1));
	0: I < M ->> on(disc(I), disc(I+1));
	maxstep: on(disc(M), peg(3));
	maxstep: I < M ->> on(disc(I), disc(I+1)).
