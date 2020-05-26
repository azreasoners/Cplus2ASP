% File 'zoo-example-bigger.cp' : specific landscape, scaled up

% landscape specific to a scenario
% The number of positions, locations, gates may be different among scenarios.

:- macros                         
  numberOfPositions -> 24.

:- objects
  1..numberOfPositions        :: position;
  cageA,cageB,cageC,cageD     :: cage;
  gateAO,gateBO,gateCO,gateDO :: gate.

%%% Scenario Landscape
%  cageA    outside      cageC
%  --------          ---------
% |  1  2  |  9 10 co  17 18  |
% |  3  4 ao 11 12  |  19 20  |
%  --------          ---------
% |  5  6 bo 13 14  |  21 22  |
% |  7  8  | 15 16 do  23 24  |
%  --------          ---------
%  cageB                 cageD
%%%


% a position is in the outside location by default
default loc(P)=outside.

caused loc(1)=cageA.
caused loc(2)=cageA.
caused loc(3)=cageA.
caused loc(4)=cageA.
caused loc(5)=cageB.
caused loc(6)=cageB.
caused loc(7)=cageB.
caused loc(8)=cageB.
caused loc(17)=cageC.
caused loc(18)=cageC.
caused loc(19)=cageC.
caused loc(20)=cageC.
caused loc(21)=cageD.
caused loc(22)=cageD.
caused loc(23)=cageD.
caused loc(24)=cageD.

% The neighbor relation is symmetric (lmw)
caused neighbor(P1,P) if neighbor(P,P1) where P<P1.

% Two positions in different locations are neighbors 
%  if they are the sides of a gate 
caused neighbor(P,P1)
   if loc(P)\=loc(P1) && [\/G | sides(P,P1,G)].

caused neighbor(1,2).
caused neighbor(1,3).
caused neighbor(1,4).
caused neighbor(2,3).
caused neighbor(2,4).
caused neighbor(3,4).

caused neighbor(5,6).
caused neighbor(5,7).
caused neighbor(5,8).
caused neighbor(6,7).
caused neighbor(6,8).
caused neighbor(7,8).

caused neighbor(9,10).
caused neighbor(9,11).
caused neighbor(9,12).
caused neighbor(10,11).
caused neighbor(10,12).
caused neighbor(11,12).
caused neighbor(11,13).
caused neighbor(11,14).
caused neighbor(12,13).
caused neighbor(12,14).
caused neighbor(13,14).
caused neighbor(13,15).
caused neighbor(13,16).
caused neighbor(14,15).
caused neighbor(14,16).
caused neighbor(15,16).

caused neighbor(17,18).
caused neighbor(17,19).
caused neighbor(17,20).
caused neighbor(18,19).
caused neighbor(18,20).
caused neighbor(19,20).

caused neighbor(21,22).
caused neighbor(21,23).
caused neighbor(21,24).
caused neighbor(22,23).
caused neighbor(22,24).
caused neighbor(23,24).

caused side1(gateAO)=4.
caused side2(gateAO)=11.

caused side1(gateBO)=6.
caused side2(gateBO)=13.

caused side1(gateCO)=10.
caused side2(gateCO)=17.

caused side1(gateDO)=16.
caused side2(gateDO)=23.
