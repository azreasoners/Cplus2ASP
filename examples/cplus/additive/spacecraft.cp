% File 'spacecraft.cp'

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
  force(jet,axis)                :: exogenousAction(integer).


always -fire(J) ->> force(J,Ax)=0.

fire(J) increments vel(Ax) by V // mass if force(J,Ax) = V.


caused pos(Ax) = P+((V+V1)//2) if vel(Ax) = V1
   after vel(Ax) = V & pos(Ax) = P where (V+V1) mod 2 = 0 &
                                         P+((V+V1)//2) >= -maxAdditive &
                                         P+((V+V1)//2) =< maxAdditive.

nonexecutable fire(J) if force(J,Ax) mod mass \= 0.

nonexecutable fire(J) if abs(force(J,Ax)) > maxForce.




