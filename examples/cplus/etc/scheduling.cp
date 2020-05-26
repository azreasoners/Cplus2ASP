% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% File: 'scheduling.obc': A Scheduling Problem
% Author: Joseph Babb
% Date: 10/07/2012
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% The agent is placed in charge of a number of
% machines specialized to perform a specific set
% of jobs. The agent must allocate the resources
% provided by each machine in order to finish
% each request job. Meanwhile, machines may
% fail or be repaired.
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Based on the example provided with:
%             oClingo v. 3.0.92
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

:- sorts
  machines;
  jobs;
  nums.

:- objects
  m1, m2	:: machines;
  j1,j2,j3,j4 	:: jobs;
  0..5 	:: nums.

:- variables
  M, M1 	:: machines;
  J, J1 	:: jobs;
  N, N1 	:: nums.

:- constants
  remaining(jobs) 				:: simpleFluent(nums);				% The number of work units a job has left.
  size(jobs) 					:: simpleFluent(nums);				% The initial size of a job.
  assignment(jobs) 				:: simpleFluent(machines*);			% The machine a job is assigned to.
  throughput(machines) 			:: simpleFluent(nums);				% The current throughput of a machine.
  depends(jobs, jobs)			:: simpleFluent;					% Dependencies amongst jobs.
  delegate(jobs) 				:: exogenousAction(machines*).		% Assigning a job to a machine.


%inertia
inertial remaining(J).
inertial size(J).
inertial assignment(J).
inertial throughput(M).
inertial depends(J,J1).

% effect of assigning a job
delegate(J)=M causes assignment(J)=M.

%a job must be restarted if its assigned machine is changed
delegate(J)=M causes remaining(J)=N if size(J)=N.

% cannot assign a job to a machine that is offline
nonexecutable delegate(J)=M if throughput(M)=0.

% Cannot a assign a completed job.
nonexecutable delegate(J)=M if remaining(J)=0.

% Cannot execute a job with an unfinished dependency
nonexecutable delegate(J)=M if depends(J,J1) & -remaining(J1)=0.

% cannot have one machine assigned to multiple jobs
constraint (assignment(J)=M & assignment(J1)=M) ->> false where J < J1.

% assigned jobs are executed as long as their machine is online
caused remaining(J)=N 
	after delegate(J)=none & remaining(J)=N+N1 
		  & assignment(J)=M & throughput(M)=N1.
caused remaining(J)=0 
	after delegate(J)=none & remaining(J)=N 
		  & assignment(J)=M & throughput(M)=N1 
	where N < N1.

% a job is unassigned when it's complete
caused assignment(J)=none if remaining(J)=0.

:- hide all.
:- show delegate(J)=M.
:- show remaining(J)=N.

%Query:
:- query
	label::0;
	0: size(j1)=1 & size(j2)=2 & size(j3)=3 & size(j4)=4;
	0: depends(j3, j2) & depends(j4, j2);
	0: (((J \= j3) & (J \= j4)) ++ J1 \= j2 ) ->> -depends(J, J1);
	0: remaining(J)=size(J);
	0: throughput(m1)=2 & throughput(m2)=4;
	0: assignment(J)=none;
    maxstep: remaining(J)=0.
