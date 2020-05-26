% File 'zoo-bigger-test1': The Big Cage Shuffle

:- include 'zoo/landscape.cp'.
:- include 'zoo/animals.cp'.
:- include 'zoo/movement.cp'.
:- include 'zoo/actions.cp'.
:- include 'zoo/example.cp'.

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

caused -largeSpecies(dogSpecies).


:- objects
  homer                        :: human;
  jumbo                        :: elephant;
  silver                       :: horse;
  snoopy                       :: dog.


% All gates are closed, there's an animal in each cage 
% except cage A, and homer is outside. 
% How quickly can everyone visit every cage?

:- variables
  T      :: step.

:- constants
	visited(animal, cage) :: inertialFluent.

caused visited(ANML, C) if loc(P)=C & pos(ANML)=P.

:- show pos(ANML); visited(ANML, C).

:- query 
label :: 0;
0: -opened(G),
	(loc(P)=C & pos(ANML)=P) <-> visited(ANML, C),
    pos(homer)=11,
    pos(jumbo)=8,
    pos(silver)=20,
    pos(snoopy)=21;
maxstep: visited(ANML, C).


