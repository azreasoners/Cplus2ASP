% file 'bw-cost.cp'.

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

:- constants
  cost            :: additiveFluent(0..maxAdditive). 

move(B) increments cost by 1.
