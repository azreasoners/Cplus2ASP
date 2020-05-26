:- maxAdditive :: 19.

:- macros
  maxCost -> 18.

:- sorts
  location >> block.

:- objects
  table                     :: location.

:- constants
  loc(block)                :: inertialFluent(location);
  move(block)               :: exogenousAction;
  destination(block)        :: attribute(location) of move(block).

:- variables
  B,B1                      :: block;
  L                         :: location.

% effect of moving a block
move(B) causes loc(B)=L if destination(B)=L.

% a block can be moved only when it is clear
nonexecutable move(B) if loc(B1)=B.

% a block can be moved only to a position that is clear
nonexecutable move(B) 
   if destination(B)=loc(B1) & destination(B)\=table.

% a block can't be moved onto a block that is being moved also
nonexecutable move(B) & move(B1) if destination(B)=B1.

% two blocks can't be on the same block at the same time
constraint loc(B)=loc(B1) ->> loc(B)=table where B @< B1.  

:- sorts
	range.

:- objects
	0..maxAdditive :: range.

:- constants
  cost            :: additiveFluent(range). 

move(B) increments cost by 1.

:- variables
  N                              :: range.

:- objects
  a,b,c,d,e,f,g,h,i,j,k,l,m,n,o  :: block.

:- show cost=N; move(B); destination(B)=L.

:- query
maxstep :: 0..100;
0:      (cost = 0  &  loc(m) = table  &  loc(l) = m
        &  loc(a) = l  &  loc(b) = a  &  loc(c) = b
        &  loc(o) = table  &  loc(n) = o  &  loc(d) = n
        &  loc(e) = d  &  loc(j) = e  &  loc(k) = j
        &  loc(f) = table  &  loc(g) = f  &  loc(h) = g
        &  loc(i) = h);
maxstep: ([\/N| cost = N & N=<maxCost]
        &  loc(e) = j  &  loc(a) = e  &  loc(n) = a
        &  loc(i) = d  &  loc(h) = i  &  loc(m) = h
        &  loc(o) = m  &  loc(k) = g  &  loc(c) = k
        &  loc(b) = c  &  loc(l) = b).


