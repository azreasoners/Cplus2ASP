% Towers of Hanoi

:- sorts
	loc >> peg; loc >> disk.


:- macros
	NUM_DISKS -> 6.

:- objects
	1..NUM_DISKS :: disk;
	p1,p2,p3 :: peg.


:- constants
	on(disk) :: inertialFluent(loc);
	move(disk) :: exogenousAction(loc*).

:- variables
	D, D1 :: disk;
	P, P1 :: peg;
	L, L1 :: loc.

% after moving disk it is at new place
caused on(D) = L after move(D) = L.

% no concurrency
nonexecutable move(D)=L & move(D1)=L1 where D\=D1.

% only move disk to location that doesn't have any other disk on top
nonexecutable move(D)=L if on(D1)=L where D\=D1.

% only move disk when it is the topmost disk
nonexecutable move(D)=L if on(D1)=D where D\=D1.

% only move disk on top of bigger disk
nonexecutable move(D)=D1 where D >= D1.

:- query
	label::0;
	maxstep:: 0..100;
	0: on(NUM_DISKS)=p1;
	0: D < NUM_DISKS ->> on(D)=D+1;
	maxstep: on(NUM_DISKS)=p3;
	maxstep: D < NUM_DISKS ->> on(D)=D+1.


