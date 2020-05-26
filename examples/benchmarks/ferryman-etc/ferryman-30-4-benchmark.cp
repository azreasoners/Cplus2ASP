% 3 sheep an three wolves shall cross a river in a boat
% which carries at most 2 animals at once. 
% On either side, the wolves may never outnumber the sheep
% or the sheep will be eaten.

:- sorts
	int;
	type.

:- macros
	NUM -> 30;
	CAPACITY -> 4.

:- objects
	0..NUM :: int;
	sheep, wolf :: type.

:- constants
	cross(int, int) :: exogenousAction;
	here(type) :: inertialFluent(int);
	boat_here :: inertialFluent.

:- variables
	S, S1, HS, S_new :: int;
	W, W1, HW, W_new :: int;
	N, N1 :: int.


% don't cross with more sheep than still here
nonexecutable cross(S, W) if boat_here & here(sheep)=HS where HS < S.

% don't cross with more wolves than still here
nonexecutable cross(S, W) if boat_here & here(wolf)=HW where HW < W.

% don't cross when boat not here and more than n sheep
nonexecutable cross(S, W) if -boat_here & here(sheep)=HS where S + HS > NUM.

% don't cross when boat not here and more than n wolves
nonexecutable cross(S, W) if -boat_here & here(wolf)=HW where  W + HW > NUM.  

% don't cross with an empty boat
nonexecutable cross(S, W) where S == 0 & W == 0.

% don't cross with more than m animals
nonexecutable cross(S, W) where S + W > CAPACITY.

% update animal count
caused -boat_here after cross(S,W) & boat_here. 
caused boat_here after cross(S,W) & -boat_here.

cross(S, W) causes here(sheep)=S_new if boat_here & here(sheep)=HS where S_new + S == HS.
cross(S, W) causes here(wolf)=W_new if boat_here & here(wolf)=HW where W_new + W == HW.

cross(S, W) causes here(sheep)=S_new if -boat_here & here(sheep)=HS where HS + S == S_new. 
cross(S, W) causes here(wolf)=W_new if -boat_here & here(wolf)=HW where HW + W == W_new. 

% don't allow less sheep than wolves when still sheeps here
caused false if here(wolf)=N & here(sheep)=N1 where 0 < N1 & N1 < N. 
% don't allow less wolves than sheeps when less then n sheeps here
caused false if here(wolf)=N & here(sheep)=N1 where N1 < NUM & N < N1.

:- query
	label::0;
	maxstep:: 1..100;
	0: here(wolf)=NUM & here(sheep)=NUM;
	0: boat_here;
	maxstep: here(wolf)=0 & here(sheep)=0.

