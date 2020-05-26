% WARNING: F2LP arrow '<-' found. Treating '-' as classical negation. Please use the option -d if you would like '-' to be treated as default negation.
% Standard description-independent declarations and laws to CCalc-ify ASP

% A derived binary relation between a constant and its domain objects,
% connected via constant_sort and sort_object

%-------------------------------------------------------------
#base.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%-------------------------------------------------------------

query_label(query) :- true.
% Selects which query to activate in a based on the value of the const "query"

constant_object(V_constant,Object) :- constant_sort(V_constant,V_sort),sort_object(V_sort,Object).

% constant_posobject relates constants with their positive objects
% positive objects are defined to be any object that is not 'none'
% (multi-valued) or 'false' (boolean)

constant_posobject(V_constant,Object) :- constant_object(V_constant,Object),constant_sort(V_constant,boolean),Object!=false.

constant_posobject(V_constant,Object) :- constant_object(V_constant,Object),not constant_sort(V_constant,boolean),Object!=none.

% constant_negobject relates constants with their negative object
% negative objects are false (boolean) and none (non-boolean)
constant_negobject(V_constant,false) :- constant_sort(V_constant,boolean).

constant_negobject(V_constant,none) :- not constant_sort(V_constant,boolean),constant(V_constant).

%-------------------------------------------------------------
#base.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%-------------------------------------------------------------

% Standard, description-independent declarations of sorts and domains

sort(boolean) :- true.
#domain boolean(V_boolean).
sort_object(boolean,V_boolean) :- true.

boolean(true) :- true.
boolean(false) :- true.

sort(computed) :- true.
#domain computed(V_computed).
sort_object(computed,V_computed) :- true.

% Constants hierarchy

% Meta-constants to group categories of constants

sort(constant) :- true.
#domain constant(V_constant).
sort_object(constant,V_constant) :- true.

%----

% Rigid constants
sort(rigid) :- true.
#domain rigid(V_rigid).
sort_object(rigid,V_rigid) :- true.

%---

% Fluent-based constants

sort(fluent) :- true.
#domain fluent(V_fluent).
sort_object(fluent,V_fluent) :- true.

sort(simpleFluent) :- true.
#domain simpleFluent(V_simpleFluent).
sort_object(simpleFluent,V_simpleFluent) :- true.

sort(inertialFluent) :- true.
#domain inertialFluent(V_inertialFluent).
sort_object(inertialFluent,V_inertialFluent) :- true.

sort(sdFluent) :- true.
#domain sdFluent(V_sdFluent).
sort_object(sdFluent,V_sdFluent) :- true.

sort(abFluent) :- true.
#domain abFluent(V_abFluent).
sort_object(abFluent,V_abFluent) :- true.

%---

% Action-based constants

sort(action) :- true.
#domain action(V_action).
sort_object(action,V_action) :- true.

sort(exogenousAction) :- true.
#domain exogenousAction(V_exogenousAction).
sort_object(exogenousAction,V_exogenousAction) :- true.

sort(abAction) :- true.
#domain abAction(V_abAction).
sort_object(abAction,V_abAction) :- true.

sort(attribute) :- true.
#domain attribute(V_attribute).
sort_object(attribute,V_attribute) :- true.

% A regular action is defined to be any action which isn't an abAction or attribute
sort(regularAction) :- true.
#domain regularAction(V_regularAction).
sort_object(regularAction,V_regularAction) :- true.

%---

% Abnormality constants

sort(abnormality) :- true.
#domain abnormality(V_abnormality).
sort_object(abnormality,V_abnormality) :- true.

sort(staticAbnormality) :- true.
#domain staticAbnormality(V_staticAbnormality).
sort_object(staticAbnormality,V_staticAbnormality) :- true.

sort(dynamicAbnormality) :- true.
#domain dynamicAbnormality(V_dynamicAbnormality).
sort_object(dynamicAbnormality,V_dynamicAbnormality) :- true.

%---

% Subsort relations

constant(V_fluent) :- true.
constant(V_action) :- true.
constant(V_rigid) :- true.
constant(V_abnormality) :- true.
fluent(V_simpleFluent) :- true.
simpleFluent(V_inertialFluent) :- true.
fluent(V_sdFluent) :- true.
sdFluent(V_abFluent) :- true.
action(V_exogenousAction) :- true.
action(V_abAction) :- true.
action(V_attribute) :- true.

regularAction(V_action) :- not attribute(V_action),not abAction(V_action).

abnormality(V_staticAbnormality) :- true.
abnormality(V_dynamicAbnormality) :- true.

% Note: This isn't necessary for abnormality constants


%-------------------------------------------------------------
#base.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%-------------------------------------------------------------

%%% Description-independent rules to CCalc-ify ASP @ t=0

step(0) :- true.

%% Handle external abnormality constants
#external ab_h(V_staticAbnormality,0).

% Exogeneity for exogenous actions
exogenous(V_exogenousAction) :- true.

% Exogeneity for attributes
exogenous(V_attribute) :- true.

% Inertia for inertial fluents
inertial(V_inertialFluent) :- true.

% Default false for abFluents
{h(eql(V_abFluent,X),0)} :- constant_negobject(V_abFluent,X).

% anyInitialState: Exogeneity for simple fluents at time 0
{h(eql(V_simpleFluent,Object),0)} :- constant_object(V_simpleFluent,Object).


% exogenous: Grants exogeneity to a constant

% Rigids
{h(eql(V_rigid,Value))} :- exogenous(V_rigid),constant_object(V_rigid,Value).

% Fluents (@ t=0)
{h(eql(V_fluent,Value),0)} :- exogenous(V_fluent),constant_object(V_fluent,Value).

% Existence and uniqueness for every constant relative to its domain

% Rigid
 :- not 1{h(eql(V_rigid,Object1)):constant_object(V_rigid,Object1)}1.

% Fluents (@ t=0)
 :- not 1{h(eql(V_fluent,Object1),0):constant_object(V_fluent,Object1)}1.


%-------------------------------------------------------------
#cumulative t.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%-------------------------------------------------------------
% Keep track of steps we've visited
step(t) :- true.
astep(t-1) :- true.

%%% Description-independent rules to CCalc-ify ASP @ t>0

%% Handle external abnormality constants
#external ab_h(V_staticAbnormality,t).
#external ab_occ(V_dynamicAbnormality,t-1).

% abActions default to false
{occ(eql(V_abAction,X),t-1)} :- constant_negobject(V_abAction,X).

% Default false for abFluents
{h(eql(V_abFluent,X),t)} :- constant_negobject(V_abFluent,X).

% Restriction that attributes must take on the value "none" if and only if their linked action does not execute
 :- {not occ(eql(V_attribute,NegObj1),t-1)}0,constant_negobject(V_action,NegObj2),action_attribute(V_action,V_attribute),constant_negobject(V_attribute,NegObj1),not occ(eql(V_action,NegObj2),t-1).
 :- {not occ(eql(V_action,NegObj2),t-1)}0,constant_negobject(V_action,NegObj2),action_attribute(V_action,V_attribute),constant_negobject(V_attribute,NegObj1),not occ(eql(V_attribute,NegObj1),t-1).

% exogenous: Grants exogeneity to a constant

% Actions
{occ(eql(V_action,Value),t-1)} :- exogenous(V_action),constant_object(V_action,Value).

% Fluents
{h(eql(V_fluent,Value),t)} :- exogenous(V_fluent),constant_object(V_fluent,Value).

% inertial: Grants inertia to a (non-rigid) fluent

{h(eql(V_fluent,Value),t)} :- inertial(V_fluent),h(eql(V_fluent,Value),t-1),constant_object(V_fluent,Value).

% noconcurrency: If stated as a fact, prevents concurrent execution of Boolean actions

 :- noconcurrency,2{occ(eql(X_action,true),t-1):action(X_action)}.

% strong_noconcurrency: A stronger version of noconcurrency which prevents 2 of any non-attribute, non abAction 
% action from occurring simultaneously

 :- strong_noconcurrency,2{occ(eql(X_action,X_Object),t-1):regularAction(X_action):constant_posobject(X_action,X_Object)}.

% Existence and uniqueness for every constant relative to its domain

% Fluents (@ t>0)
 :- not 1{h(eql(V_fluent,Object1),t):constant_object(V_fluent,Object1)}1.

% Actions
 :- not 1{occ(eql(V_action,Object1),t-1):constant_object(V_action,Object1)}1.

%-------------------------------------------------------------
#volatile t.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%-------------------------------------------------------------

% Visit at least minstep before returning a solution
  :- not step(minstep).


%-------------------------------------------------------------
#base.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%-------------------------------------------------------------

% Etc
  #hide step/1.
#hide astep/1.
#hide constant_object/2.
#hide constant_posobject/2.
#hide constant_negobject/2.
#hide constant_sort/2.
#hide sort_object/2.
#hide action_attribute/2.
#hide sort/1.
#hide sort_object/2.
#hide boolean/1.
#hide constant/1.
#hide fluent/1.
#hide simpleFluent/1.
#hide inertialFluent/1.
#hide sdFluent/1.
#hide abFluent/1.
#hide action/1.
#hide exogenousAction/1.
#hide abAction/1.
#hide attribute/1.
#hide regularAction/1.
#hide abnormality/1.
#hide staticAbnormality/1.
#hide dynamicAbnormality/1.
#hide exogenous/1.
#hide inertial/1.
#hide noconcurrency.
#hide strong_noconcurrency.
#hide query_label/1.
#hide rigid/1.
#hide computed/1.


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% File "/home/jbabb1/workspace/cplus2asp/examples/bc/bwbc"

#base.
sort(block) :- true.
#domain block(V_block).
sort_object(block,V_block) :- true.
#hide block(V_block).

loc(V_block) :- true.
sort(loc) :- true.
#domain loc(V_loc).
sort_object(loc,V_loc) :- true.
#hide loc(V_loc).


block(b1) :- true.
block(b2) :- true.
block(b3) :- true.
block(b4) :- true.
loc(table) :- true.

inertialFluent(loc(V_block)) :- true.
constant_sort(loc(V_block),loc) :- true.
sdFluent(in_tower(V_block)) :- true.
constant_sort(in_tower(V_block),boolean) :- true.
exogenousAction(move(V_block,V_loc)) :- true.
constant_sort(move(V_block,V_loc),boolean) :- true.

#domain block(B).
#domain block(B1).
#domain block(B2).
#domain loc(L).
#domain loc(L1).

% location
 :- h(eql(loc(B1),B),0),h(eql(loc(B2),B),0),B1!=B2.
#cumulative maxstep.
 :- h(eql(loc(B1),B),maxstep),h(eql(loc(B2),B),maxstep),B1!=B2.

% Definition of a tower
#base.
h(eql(in_tower(B),false),0) :- {not h(eql(in_tower(B),false),0)}0,true.
#cumulative maxstep.
h(eql(in_tower(B),false),maxstep) :- {not h(eql(in_tower(B),false),maxstep)}0,true.
#base.
h(eql(in_tower(B),true),0) :- h(eql(loc(B),table),0).
#cumulative maxstep.
h(eql(in_tower(B),true),maxstep) :- h(eql(loc(B),table),maxstep).
#base.
h(eql(in_tower(B),true),0) :- h(eql(loc(B),B1),0),h(eql(in_tower(B1),true),0).
#cumulative maxstep.
h(eql(in_tower(B),true),maxstep) :- h(eql(loc(B),B1),maxstep),h(eql(in_tower(B1),true),maxstep).

% Blocks don't float in the air
#base.
 :- h(eql(in_tower(B),false),0).
#cumulative maxstep.
 :- h(eql(in_tower(B),false),maxstep).

% Moving a block
h(eql(loc(B),L),maxstep) :- occ(eql(move(B,L),true),maxstep-1).

 :- occ(eql(move(B,L),true),maxstep-1),h(eql(loc(B1),B),maxstep-1).

% [Query: Label:stack]
#base.
 :- query_label(stack),not h(eql(loc(B),table),0).
#volatile maxstep.
 :- not h(eql(loc(b4),b3),maxstep),query_label(stack).
 :- not h(eql(loc(b3),b2),maxstep),query_label(stack).
 :- not h(eql(loc(b2),b1),maxstep),query_label(stack).
#base.
true.
:- false.
#hide true/0.
