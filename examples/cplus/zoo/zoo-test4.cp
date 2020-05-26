% File 'zoo-test4'

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
  jumbo                        :: elephant.

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

% Initially Homer was outside, and Jumbo, an elephant, was inside the cage, 
% with the gate closed. How many steps are required to switch their locations?

:- query 
label :: 0;
maxstep :: 3..10;
0: -opened(gateAO),
   loc(pos(homer))=outside,
   loc(pos(jumbo))=cageA;
maxstep: 
   loc(pos(homer))=cageA,
   loc(pos(jumbo))=outside.


% If Homer has never mounted Jumbo in the above query, will the answers 
% be the same?

:- variables
  T :: step.

:- query 
label :: 1;
maxstep :: 4..10;
0: -opened(gateAO),
   loc(pos(homer))=outside,
   loc(pos(jumbo))=cageA;
maxstep: 
   loc(pos(homer))=cageA,
   loc(pos(jumbo))=outside;
[/\T /\ANML | (T: -mounted(homer,ANML))].

% the last line may be replaced with
% V_step: -mounted(homer,ANML).


