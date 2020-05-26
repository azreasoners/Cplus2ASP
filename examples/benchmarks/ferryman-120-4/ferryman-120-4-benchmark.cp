% 3 sheep an three wolves shall cross a river in a boat
% which carries at most 2 animals at once. 
% On either side, the wolves may never outnumber the sheep
% or the sheep will be eaten.

:- sorts
	int;
	small_int;
	big_int;
	type.

:- macros
	NUM -> 120;
	CAPACITY -> 4.

:- objects
	0..NUM :: int;
	0..CAPACITY :: small_int;
	NUM-CAPACITY..NUM :: big_int;
	sheep, wolf :: type.

:- constants
	cross(small_int, small_int) :: exogenousAction;
	here(type) :: inertialFluent(int);
	boat_here :: inertialFluent.

:- variables
	SS, SS1, SW, SW1 :: small_int;
	BS, BS1, BW, BW1 :: big_int;
	HS :: int;
	HW :: int;
	N, N1 :: int.


% don't cross with more sheep than still here
nonexecutable cross(SS, SW) if boat_here & here(sheep)=SS1 where SS1 < SS.

% don't cross with more wolves than still here
nonexecutable cross(SS, SW) if boat_here & here(wolf)=SW1 where SW1 < SW.

% don't cross when boat not here and more than n sheep
nonexecutable cross(SS, SW) if -boat_here & here(sheep)=BS where SS + BS > NUM.

% don't cross when boat not here and more than n wolves
nonexecutable cross(SS, SW) if -boat_here & here(wolf)=BW where  SW + BW > NUM.  

% don't cross with an empty boat
nonexecutable cross(0, 0). 

% don't cross with more than m animals
nonexecutable cross(SS, SW) where SS + SW > CAPACITY.

% update animal count
caused -boat_here after cross(SS,SW) & boat_here. 
caused boat_here after cross(SS,SW) & -boat_here.

cross(SS, SW) causes here(sheep)=HS - SS if boat_here & here(sheep)=HS where HS - SS >= 0.
cross(SS, SW) causes here(wolf)=HW - SW if boat_here & here(wolf)=HW where HW - SW >= 0.

cross(SS, SW) causes here(sheep)=HS+SS if -boat_here & here(sheep)=HS where HS + SS < 121. 
cross(SS, SW) causes here(wolf)=HW+SW if -boat_here & here(wolf)=HW where HW + SW < 121. 

% don't allow less sheep than wolves when still sheeps here
caused false if here(wolf)=N & here(sheep)=N1 where 0 < N1 & N1 < N. 
% don't allow less wolves than sheeps when less then n sheeps here
caused false if here(wolf)=N & here(sheep)=N1 where N1 < NUM & N < N1.

:- query
	label::0;
	maxstep:: 1..600;
	0: here(wolf)=NUM & here(sheep)=NUM;
	0: boat_here;
	maxstep: here(wolf)=0 & here(sheep)=0.

