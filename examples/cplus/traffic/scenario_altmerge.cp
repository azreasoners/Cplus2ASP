% File 'scenario_altmerge.cp', the Alternate Merging scenario.

:- macros
  maxInt -> 12;
  varsigma -> 2;
  carsPerLane -> 3.

:- include 'traffic.cp'.

:- macros
  segNode(#1,#2,#3) -> (startNode(#1)=(#2) & endNode(#1)=(#3)).

%%%
% In this scenario, there are two lanes of traffic that merge together into one.
% All of the cars in a lane are painted the same color (red or black).
% Each lane of cars is initially identical w.r.t. car positions.
% The cars must merge onto the common lane in such a way that the colors of 
% the cars alternate between red and black in the final configuration.
%
% --RC1----RC2----RC3--
%                      \
%                      ------------------------------------------
%                      /
% --BC1----BC2----BC3--
%
% It's expected that there will be two solutions: one with a red car in front:
%
% ---------------------
%                      \
%                       -BC1----RC1----BC2----RC2----BC3----RC3--
%                      /
% ---------------------
%
% and one with a black car in front (same as above, swap colors).
%%%

:- sorts
  carNum.

:- objects
  1..carsPerLane :: carNum.

:- variables
  CN,CN1,CN2 :: carNum.

:- objects
  a,b,c,d                   :: node;
  seg_ac,seg_bc,seg_cd      :: segment;
  blackcar(carNum)          :: car;
  redcar(carNum)            :: car.

caused segNode(seg_ac,a,c).
caused segNode(seg_bc,b,c).
caused segNode(seg_cd,c,d).

caused length(seg_ac)=carsPerLane*varsigma.
caused length(seg_bc)=carsPerLane*varsigma.
caused length(seg_cd)=2*carsPerLane*varsigma.

caused speedLimit(seg_ac)=1.
caused speedLimit(seg_bc)=1.
caused speedLimit(seg_cd)=1.

caused topSpeed(blackcar(CN))=1.
caused topSpeed(redcar(CN))=2.

% All roads are one way.
constraint positiveOrientation(C).

% Cars in the final segment have to alternate colors.
constraint segment(redcar(CN))=seg_cd & segment(redcar(CN1))=seg_cd & ahead(redcar(CN),redcar(CN1)) ->> [ \/ CN2 | ahead(blackcar(CN2),redcar(CN1)) & -ahead(blackcar(CN2),redcar(CN)) ].
constraint segment(blackcar(CN))=seg_cd & segment(blackcar(CN1))=seg_cd & ahead(blackcar(CN),blackcar(CN1)) ->> [ \/ CN2 | ahead(redcar(CN2),blackcar(CN1)) & -ahead(redcar(CN2),blackcar(CN)) ].

%:- show segment(C); distance(C); speed(C);
%        nextSegment(C)=Sg; willLeave(C); node(C)=Nd.

:- query
label :: 0;
maxstep :: 11;
0: position(redcar(CN),seg_ac,varsigma*CN);
0: position(blackcar(CN),seg_bc,varsigma*CN);
0: [/\C | positiveOrientation(C)];
maxstep: [/\C | segment(C)=seg_cd].


