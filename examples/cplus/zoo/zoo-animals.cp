%%% ANIMALS %%%

:- sorts
  animal >> human;
  species.

:- variables
  ANML,ANML1                        :: animal;
  H,H1                              :: human;
  SP                                :: species.

:- objects
% One of the species is human (lmw)
  humanSpecies                      :: species.

:- constants
% Each animal belongs to exactly one of a number of species. 
%  (lmw)  Membership of an animal in a species does not change 
%  over time (lmw)
  sp(animal)                        :: species;
% Some species are large, some are not (lmw)
  largeSpecies(species)             :: boolean;
% Each animal has a position at each point in time (lmw)
  pos(animal)                       :: inertialFluent(position);
% Boolean properties of animals (lmw)
  adult(animal)                     :: boolean;
  mounted(human,animal)             :: inertialFluent.

default largeSpecies(SP).
default adult(ANML).

% Humans are a species called humanSpecies
caused sp(H)=humanSpecies.
constraint sp(ANML)=humanSpecies ->> [\/H | ANML=H].

:- constants
  large(animal) :: boolean.

default -large(ANML).
% Adult members of large species are large animals (lmw)
caused large(ANML) if adult(ANML) & (sp(ANML)=SP & largeSpecies(SP)).
:- begin_asp.
#hide h(eql(large(_),_)).
:- end_asp.

% There is at least one human-species animal in each scenario 
%  (lmw) 
constraint [\/H | true].  

% Two large animals can not occupy the same position,
%  except if one of them rides on the other (lmw)
constraint pos(ANML)=pos(ANML1) & large(ANML) & large(ANML1)
     ->> [\/H | (H=ANML & mounted(H,ANML1)) ++
                (H=ANML1 & mounted(H,ANML))] where ANML@<ANML1.
