% File 'spacecraft':  "Describing Additive Fluents in C+", Section 7
:- maxAdditive :: 25.

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

fire(J) increments vel(Ax) by V // mass if force(J,Ax) = V.


caused pos(Ax) = P+((V+V1)//2) if vel(Ax) = V1
   after vel(Ax) = V & pos(Ax) = P where (V+V1) mod 2 == 0 &
                                         P+((V+V1)//2) >= -maxAdditive &
                                         P+((V+V1)//2) =< maxAdditive.

nonexecutable fire(J) if abs(force(J,Ax)) > maxForce.

%:- show pos(Ax); vel(Ax).

:- query
 label:: 0;
 maxstep:: 0..100;
 0: (pos(x) = 0  &  pos(y) =  0  &  pos(z) =  0);
 0: (vel(x) =  0  &  vel(y) =  0  &  vel(z) =  0);
 maxstep: (pos(x) = 10  &  pos(y) = 10  &  pos(z) = 10);
 maxstep: (vel(x) = 0 & vel(y) = 0 & vel(z) = 0).

