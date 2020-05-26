% 3 sheep an three wolves shall cross a river in a boat
% which carries at most 2 animals at once. 
% On either side, the wolves may never outnumber the sheep
% or the sheep will be eaten.

% LONG Elaboration: There are, in fact, SMAX shorelines. 
% The goal is to get to #SMAX.
% No-one can be left behind, the boat may only go to S+1
% the next shore when everyone is off S-1.

:- sorts
	int;
	type;
	shores;
	not_initial_shores;
	dir.

:- macros
	NUM -> 15;
	CAPACITY -> 4;
	SMAX -> 3.

:- objects
	0..NUM :: int;
	sheep, wolf :: type;
	1..SMAX :: shores;
	2..SMAX :: not_initial_shores;
	forward, backward :: dir.

:- constants
	cross			:: exogenousAction(dir);
	carrying(type)	:: exogenousAction(int);
	at(shores,type) :: inertialFluent(int);
	boat_at 		:: inertialFluent(shores).

:- variables
	I, I1, I2, I_new :: int;
	T :: type;
	SH,SH1 :: shores;
	NS :: not_initial_shores;
	D :: dir.

% don't cross with more of a type than still on the shore.
nonexecutable 
	carrying(T)=I
	if boat_at=SH & at(SH, T)=I2 where I > I2.

% don't cross with an empty boat
nonexecutable [/\T| carrying(T)=0].

% don't cross with more than m animals
nonexecutable carrying(sheep)=I & carrying(wolf)=I1
 	where I + I1 > CAPACITY.

% Don't move off the map.
nonexecutable cross=forward if boat_at=SMAX.
nonexecutable cross=backward if boat_at=1.

% don't leave animals behind.
nonexecutable cross=forward if boat_at=SH & at(SH-1, T) > 0.
nonexecutable cross=backward if boat_at=SH & at(SH+1, T) > 0.

% Update locations
caused boat_at=SH+1 after cross=forward & boat_at=SH.
caused boat_at=SH-1 after cross=backward & boat_at=SH.
caused at(SH, T)=I+I1 if boat_at=SH after at(SH, T)=I & carrying(T)=I1.
caused at(SH, T)=I-I1 after at(SH, T)=I & boat_at=SH & carrying(T)=I1.

% don't allow less sheep than wolves when still sheeps here
caused false if at(SH, wolf)=I & at(SH, sheep)=I1 where 0 < I1 & I1 < I. 

:- query
	label::0;
%	maxstep:: 1..300;
	maxstep:: 1..60;
%	maxstep:: 1..30;
	0: at(1, wolf)=NUM & at(1, sheep)=NUM;
	0: at(NS, wolf)=0 & at(NS, sheep)=0;
	0: boat_at=1;
	maxstep: at(SMAX, wolf)=NUM & at(SMAX, sheep)=NUM.

