% File 'zoo-test2.cp'

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
  homer                        :: human.

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

% The gate was closed, and Homer was outside; after two steps,
% he was inside.  What can we say about his initial position?

:- query 
maxstep :: 2;
0: -opened(gateAO),
  (pos(homer)=V_position ->> loc(V_position)=outside);
maxstep: (pos(homer)=V_position ->> loc(V_position)=cageA).


