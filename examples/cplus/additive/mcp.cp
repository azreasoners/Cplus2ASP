% File 'mcp.cp'

:- sorts
  vessel;  location;  group.

:- variables
  V                          :: vessel;
  L                          :: location;
  G                          :: group;
  N                          :: 0..3.

:- objects
  boat1,boat2                :: vessel;
  bank1,bank2                :: location;
  mi,ca                      :: group.

:- constants
  loc(vessel)                :: inertialFluent(location);
  num(group,location)        :: additiveFluent(0..3);
  departing(group,location)  :: additiveAction(0..3);
  cross(vessel)              :: exogenousAction;
  to(vessel)                 :: attribute(location) of cross(vessel);
  howmany(vessel,group)      :: exogenousAction(0..3);
  capacity(vessel)           :: 1..2.

:- macros
  staying(#1,#2) -> num(#1,#2)-departing(#1,#2).

capacity(boat1)=1.
capacity(boat2)=2.

always -cross(V) ->> howmany(V,G)=0.

constraint num(mi,L)\=0 ->> num(mi,L)>=num(ca,L).
always staying(mi,L)\=0 ->> staying(mi,L)>=num(ca,L).
always num(G,L)>=departing(G,L).

cross(V) causes loc(V)=L if to(V)=L.
cross(V) decrements num(G,L) by N if howmany(V,G)=N & loc(V)=L.
cross(V) increments num(G,L) by N if howmany(V,G)=N & to(V)=L.
cross(V) increments departing(G,L) by N if howmany(V,G)=N & loc(V)=L.

nonexecutable cross(V) if loc(V)=to(V).
nonexecutable cross(V) if [/\G | howmany(V,G)=0].
nonexecutable cross(V) if howmany(V,mi)+howmany(V,ca) > capacity(V).
