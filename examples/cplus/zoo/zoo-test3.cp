% File 'zoo-test3.cp'

:- include 'zoo-landscape.cp'.
:- include 'zoo-animals.cp'.
:- include 'zoo-movement.cp'.
:- include 'zoo-actions.cp'.
:- include 'zoo-example.cp'.

:- sorts
  animal >> (elephant; horse; dog).

:- variables
  SP                           :: species;
  E                            :: elephant;
  HR                           :: horse;
  D                            :: dog.
  
:- objects
  elephantSpecies, 
  horseSpecies,
  dogSpecies                   :: species.

caused sp(E)=elephantSpecies.
constraint sp(ANML)=elephantSpecies ->> [\/E | ANML=E].

caused sp(HR)=horseSpecies.
constraint sp(ANML)=horseSpecies ->> [\/HR | ANML=HR].

caused sp(D)=dogSpecies.
constraint sp(ANML)=dogSpecies ->> [\/D | ANML=D].

default largeSpecies(SP). 
caused -largeSpecies(dogSpecies).

:- objects
  homer                        :: human;
  snoopy                       :: dog.

:- begin_asp.
#hide h(eql(accessible(_,_),_),_).
#hide h(eql(adult(_),_)).
#hide h(eql(largeSpecies(_),_)).
#hide h(eql(loc(_),_)).
#hide h(eql(neighbor(_,_),_)).
#hide h(eql(side1(_),_)).
#hide h(eql(side2(_),_)).
#hide h(eql(sides(_,_,_),_)).
#hide h(eql(sp(_),_)).
:- end_asp.

% Initially Homer was outside, and Snoopy, a dog, was inside the cage, 
% with the gate closed. Is it possible that they switched their
% locations in one step? in two steps?  

:- variables
  V_position_nest1, V_position_nest2,
  V_position_nest3, V_position_nest4 :: position.

:- query 
maxstep :: 1..2;
0: -opened(gateAO),
   (pos(homer)=V_position_nest1 ->> loc(V_position_nest1)=outside),
   (pos(snoopy)=V_position_nest2 ->> loc(V_position_nest2)=cageA);
maxstep: 
   (pos(homer)=V_position_nest3 ->> loc(V_position_nest3)=cageA),
   (pos(snoopy)=V_position_nest4 ->> loc(V_position_nest4)=outside).


