% File 'scenario1.cp'

:- macros
  maxInt -> 4;
  varsigma -> 1.

:- include 'traffic.cp'.

:- macros
  segNode(#1,#2,#3) -> (startNode(#1)=(#2) & endNode(#1)=(#3)).

:- objects
  a,b,c                     :: node;
  seg_ab,seg_bc1,seg_bc2    :: segment;
  car1                      :: car.


%caused segNode(seg_ab,a,b).
caused startNode(seg_ab)=a & endNode(seg_ab)=b.
caused segNode(seg_bc1,b,c).
caused segNode(seg_bc2,b,c).

caused length(seg_ab)=4.
caused length(seg_bc1)=4.
caused length(seg_bc2)=3.

caused speedLimit(seg_ab)=2.
caused speedLimit(seg_bc1)=2.
caused speedLimit(seg_bc2)=1.

caused topSpeed(car1)=2.

%:- show segment(C); distance(C); speed(C);
%        nextSegment(C)=Sg; node(C)=Nd; willLeave(C).


:- query
label :: 1;
maxstep :: 3..4;
0: position(car1,seg_ab,0);
0: positiveOrientation(car1);
maxstep: node(car1)=c.


:- query
label    :: 2;
maxstep  :: 5;
0: position(car1,seg_ab,0);
0: positiveOrientation(car1);
maxstep: node(car1)=c.

:- query
label :: 100;
maxstep :: 2..5;
2: chooseSegment(car1, seg_bc1).


:- query
label :: 101;
maxstep :: 2..5;
maxstep-1: chooseSegment(car1, seg_bc1).
