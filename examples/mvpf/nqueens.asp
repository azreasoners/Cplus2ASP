num(1..8).

#domain num(I).
#domain num(I1).
#domain num(J).
#domain num(J1).
#domain num(N).


:- not 1{q(I,J, V): num(V)}1.
{q(I,J,N)}.
:- q(I,J,N),q(I1,J,N),I!=I1.
:- q(I,J,N),q(I,J1,N),J!=J1.
:- q(I,J,N),q(I1,J1,N),I!=I1,#abs((I1-I))==J1-J.

