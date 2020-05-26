% File 'spacecraft':  "Describing Additive Fluents in C+", Section 7
:- maxAdditive :: 15.

:- sorts
  integer;
  axis; 
  jet.

:- objects
  -maxAdditive..maxAdditive      :: integer;
  x,y,z                          :: axis;
  jet1,jet2                      :: jet.

:- variables 
  Ax                             :: axis;
  J                              :: jet;
  F,V,V1,P                       :: integer.

:- constants
  pos(axis)                      :: simpleFluent(integer);
  vel(axis)                      :: additiveFluent(integer);
  fire(jet)                      :: exogenousAction;
  force(jet,axis)                :: attribute(integer) of fire(jet).

:- macros 
  mass -> 1;
  maxForce -> 2.

fire(J) increments vel(Ax) by V // mass if force(J,Ax) = V & V \= 0.


caused pos(Ax) = P+((V+V1)//2) if vel(Ax) = V1
   after vel(Ax) = V & pos(Ax) = P where (V+V1) mod 2 == 0 &
                                         P+((V+V1)//2) >= -maxAdditive &
                                         P+((V+V1)//2) =< maxAdditive.

nonexecutable fire(J) if abs(V) > maxForce & force(J,Ax)=V.

%:- show pos(Ax); vel(Ax).

:- query
 label:: 0;
 maxstep:: 0..4;
 0: (pos(x) = 0  &  pos(y) =  0  &  pos(z) =  0);
 0: (vel(x) =  0  &  vel(y) =  0  &  vel(z) =  0);
 maxstep: (pos(x) = 8  &  pos(y) = 8  &  pos(z) = 8);
 maxstep: (vel(x) = 0 & vel(y) = 0 & vel(z) = 0).

