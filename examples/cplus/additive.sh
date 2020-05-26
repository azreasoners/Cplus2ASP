# cplus2asp test script file.

######################################################################################################################
# Additive Domains
# --------------------------------------------------------------------------------------------------------------------

# ------------------------------------------------------ Buying ------------------------------------------------------
echo "% ------------------------------------------------------ Buying ------------------------------------------------------" 

# ------------------------- Query 1 -------------------------
echo "% ------------------------- Query 1 -------------------------" 

echo "$ cplus2asp additive/buying.query query=1  --none-alias" 
cplus2asp additive/buying.query query=1 --none-alias

# % Pragma: One or more additive constants have been detected. Including the additive standard file.
# % Pragma: Found a maxAdditive hint. Setting maxAdditive to '7'.
# % Pragma: Running Query 'Query 1'.
# % Pragma: Minimum Step = '0', Maximum Step = '1'.
# % Pragma: Searching for 1 solutions.
# Solution 1:
# 
# 	0:  has(buyer,money)=6
# 
# 	ACTIONS:  buy(magazine) buy(newspaper) howmany(magazine)=1 howmany(newspaper)=2
# 
# 	1:  has(buyer,money)=1
# 
# 
# SATISFIABLE
# Models      : 1+
# Time        : 0.010
# Prepare   : 0.000
# Prepro.   : 0.010
# Solving   : 0.000


# ------------------------- Query 2 -------------------------
echo "% ------------------------- Query 2 -------------------------" 

echo "$ cplus2asp additive/buying.query query=2  --none-alias" 
cplus2asp additive/buying.query query=2  --none-alias

# % Pragma: One or more additive constants have been detected. Including the additive standard file.
# % Pragma: Found a maxAdditive hint. Setting maxAdditive to '7'.
# % Pragma: Running Query 'Query 2'.
# % Pragma: Minimum Step = '0', Maximum Step = '1'.
# % Pragma: Searching for 1 solutions.
# % Pragma: No solutions found.
# No solution.
# 
# UNSATISFIABLE
# Models      : 0
# Time        : 0.010
# Prepare   : 0.010
# Prepro.   : 0.000
# Solving   : 0.000

# ---------------------------------------------------- Spacecraft ----------------------------------------------------
echo "% ---------------------------------------------------- Spacecraft ----------------------------------------------------" 

# ------------------------- Query 0 -------------------------
echo "% ------------------------- Query 0 -------------------------" 

echo "$ cplus2asp additive/spacecraft.query query=0 0 --none-alias" 
cplus2asp additive/spacecraft.query query=0 0 --none-alias 

# % Pragma: One or more additive constants have been detected. Including the additive standard file.
# % Pragma: Found a maxAdditive hint. Setting maxAdditive to '7'.
# % Pragma: Running Query 'Query 0'.
# % Pragma: Minimum Step = '0', Maximum Step = '1'.
# % Pragma: Searching for all solutions.
# Solution 1:
# 
# 	0:  pos(x)=-1 pos(y)=0 pos(z)=1 vel(x)=0 vel(y)=1 vel(z)=1
# 
# 	ACTIONS:  fire(jet1) fire(jet2) force(jet1,x)=0 force(jet1,y)=2 force(jet1,z)=-2 force(jet2,x)=2 force(jet2,y)=2 force(jet2,z)=0
# 
# 	1:  pos(x)=0 pos(y)=3 pos(z)=1 vel(x)=2 vel(y)=5 vel(z)=-1
# 
# 
# Solution 2:
# 
# 	0:  pos(x)=-1 pos(y)=0 pos(z)=1 vel(x)=0 vel(y)=1 vel(z)=1
# 
# 	ACTIONS:  fire(jet1) fire(jet2) force(jet1,x)=1 force(jet1,y)=2 force(jet1,z)=-2 force(jet2,x)=1 force(jet2,y)=2 force(jet2,z)=0
# 
# 	1:  pos(x)=0 pos(y)=3 pos(z)=1 vel(x)=2 vel(y)=5 vel(z)=-1
# 
# 
# Solution 3:
# 
# 	0:  pos(x)=-1 pos(y)=0 pos(z)=1 vel(x)=0 vel(y)=1 vel(z)=1
# 
# 	ACTIONS:  fire(jet1) fire(jet2) force(jet1,x)=2 force(jet1,y)=2 force(jet1,z)=-2 force(jet2,x)=0 force(jet2,y)=2 force(jet2,z)=0
# 
# 	1:  pos(x)=0 pos(y)=3 pos(z)=1 vel(x)=2 vel(y)=5 vel(z)=-1
# 
# 
# Solution 4:
# 
# 	0:  pos(x)=-1 pos(y)=0 pos(z)=1 vel(x)=0 vel(y)=1 vel(z)=1
# 
# 	ACTIONS:  fire(jet1) fire(jet2) force(jet1,x)=0 force(jet1,y)=2 force(jet1,z)=-1 force(jet2,x)=2 force(jet2,y)=2 force(jet2,z)=-1
# 
# 	1:  pos(x)=0 pos(y)=3 pos(z)=1 vel(x)=2 vel(y)=5 vel(z)=-1
# 
# 
# Solution 5:
# 
# 	0:  pos(x)=-1 pos(y)=0 pos(z)=1 vel(x)=0 vel(y)=1 vel(z)=1
# 
# 	ACTIONS:  fire(jet1) fire(jet2) force(jet1,x)=1 force(jet1,y)=2 force(jet1,z)=-1 force(jet2,x)=1 force(jet2,y)=2 force(jet2,z)=-1
# 
# 	1:  pos(x)=0 pos(y)=3 pos(z)=1 vel(x)=2 vel(y)=5 vel(z)=-1
# 
# 
# Solution 6:
# 
# 	0:  pos(x)=-1 pos(y)=0 pos(z)=1 vel(x)=0 vel(y)=1 vel(z)=1
# 
# 	ACTIONS:  fire(jet1) fire(jet2) force(jet1,x)=2 force(jet1,y)=2 force(jet1,z)=-1 force(jet2,x)=0 force(jet2,y)=2 force(jet2,z)=-1
# 
# 	1:  pos(x)=0 pos(y)=3 pos(z)=1 vel(x)=2 vel(y)=5 vel(z)=-1
# 
# 
# Solution 7:
# 
# 	0:  pos(x)=-1 pos(y)=0 pos(z)=1 vel(x)=0 vel(y)=1 vel(z)=1
# 
# 	ACTIONS:  fire(jet1) fire(jet2) force(jet1,x)=0 force(jet1,y)=2 force(jet1,z)=0 force(jet2,x)=2 force(jet2,y)=2 force(jet2,z)=-2
# 
# 	1:  pos(x)=0 pos(y)=3 pos(z)=1 vel(x)=2 vel(y)=5 vel(z)=-1
# 
# 
# Solution 8:
# 
# 	0:  pos(x)=-1 pos(y)=0 pos(z)=1 vel(x)=0 vel(y)=1 vel(z)=1
# 
# 	ACTIONS:  fire(jet1) fire(jet2) force(jet1,x)=1 force(jet1,y)=2 force(jet1,z)=0 force(jet2,x)=1 force(jet2,y)=2 force(jet2,z)=-2
# 
# 	1:  pos(x)=0 pos(y)=3 pos(z)=1 vel(x)=2 vel(y)=5 vel(z)=-1
# 
# 
# Solution 9:
# 
# 	0:  pos(x)=-1 pos(y)=0 pos(z)=1 vel(x)=0 vel(y)=1 vel(z)=1
# 
# 	ACTIONS:  fire(jet1) fire(jet2) force(jet1,x)=2 force(jet1,y)=2 force(jet1,z)=0 force(jet2,x)=0 force(jet2,y)=2 force(jet2,z)=-2
# 
# 	1:  pos(x)=0 pos(y)=3 pos(z)=1 vel(x)=2 vel(y)=5 vel(z)=-1
# 
# 
# SATISFIABLE
# Models      : 9
# Time        : 0.060
# Prepare   : 0.040
# Prepro.   : 0.020
# Solving   : 0.000

# -------------------------------------------------------- MCP -------------------------------------------------------
echo "% -------------------------------------------------------- MCP -------------------------------------------------------" 

# ------------------------- Query 0 -------------------------
echo "% ------------------------- Query 0 -------------------------" 

echo "$ cplus2asp additive/mcp.query query=0 --none-alias" 
cplus2asp additive/mcp.query query=0 --none-alias 

# % Pragma: One or more additive constants have been detected. Including the additive standard file.
# % Pragma: Found a maxAdditive hint. Setting maxAdditive to '3'.
# % Pragma: Running Query 'Query 0'.
# % Pragma: Minimum Step = '6', Maximum Step = '7'.
# % Pragma: Searching for 1 solutions.
# Solution 1:
# 
# 	0:  loc(boat1)=bank1 loc(boat2)=bank1 num(ca,bank1)=3 num(ca,bank2)=0 num(mi,bank1)=3 num(mi,bank2)=0
# 
# 	ACTIONS:  cross(boat2) howmany(boat2,ca)=2 howmany(boat2,mi)=0 to(boat2)=bank2
# 
# 	1:  loc(boat1)=bank1 loc(boat2)=bank2 num(ca,bank1)=1 num(ca,bank2)=2 num(mi,bank1)=3 num(mi,bank2)=0
# 
# 	ACTIONS:  cross(boat2) howmany(boat2,ca)=1 howmany(boat2,mi)=0 to(boat2)=bank1
# 
# 	2:  loc(boat1)=bank1 loc(boat2)=bank1 num(ca,bank1)=2 num(ca,bank2)=1 num(mi,bank1)=3 num(mi,bank2)=0
# 
# 	ACTIONS:  cross(boat2) howmany(boat2,ca)=2 howmany(boat2,mi)=0 to(boat2)=bank2
# 
# 	3:  loc(boat1)=bank1 loc(boat2)=bank2 num(ca,bank1)=0 num(ca,bank2)=3 num(mi,bank1)=3 num(mi,bank2)=0
# 
# 	ACTIONS:  cross(boat2) howmany(boat2,ca)=1 howmany(boat2,mi)=0 to(boat2)=bank1
# 
# 	4:  loc(boat1)=bank1 loc(boat2)=bank1 num(ca,bank1)=1 num(ca,bank2)=2 num(mi,bank1)=3 num(mi,bank2)=0
# 
# 	ACTIONS:  cross(boat1) cross(boat2) howmany(boat1,ca)=0 howmany(boat1,mi)=1 howmany(boat2,ca)=0 howmany(boat2,mi)=2 to(boat1)=bank2 to(boat2)=bank2
# 
# 	5:  loc(boat1)=bank2 loc(boat2)=bank2 num(ca,bank1)=1 num(ca,bank2)=2 num(mi,bank1)=0 num(mi,bank2)=3
# 
# 	ACTIONS:  cross(boat1) cross(boat2) howmany(boat1,ca)=1 howmany(boat1,mi)=0 howmany(boat2,ca)=1 howmany(boat2,mi)=0 to(boat1)=bank1 to(boat2)=bank1
# 
# 	6:  loc(boat1)=bank1 loc(boat2)=bank1 num(ca,bank1)=3 num(ca,bank2)=0 num(mi,bank1)=0 num(mi,bank2)=3
# 
# 	ACTIONS:  cross(boat1) cross(boat2) howmany(boat1,ca)=1 howmany(boat1,mi)=0 howmany(boat2,ca)=2 howmany(boat2,mi)=0 to(boat1)=bank2 to(boat2)=bank2
# 
# 	7:  loc(boat1)=bank2 loc(boat2)=bank2 num(ca,bank1)=0 num(ca,bank2)=3 num(mi,bank1)=0 num(mi,bank2)=3
# 
# 
# SATISFIABLE
# Models      : 1+
# Total Steps : 7
# Time        : 0.370
# Prepare   : 0.130
# Prepro.   : 0.010
# Solving   : 0.230


# ------------------------------------------------------ BW-Cost -----------------------------------------------------
echo "% ------------------------------------------------------ BW-Cost -----------------------------------------------------" 

# ------------------------- Query 0 -------------------------
echo "% ------------------------- Query 0 -------------------------" 

echo "$ cplus2asp additive/bw-cost.query query=0 --none-alias" 
cplus2asp additive/bw-cost.query query=0 --none-alias 

# % Pragma: One or more additive constants have been detected. Including the additive standard file.
# % Pragma: Found a maxAdditive hint. Setting maxAdditive to '19'.
# % Pragma: Running Query 'Query 0'.
# % Pragma: Minimum Step = '7', Maximum Step = '8'.
# % Pragma: Searching for 1 solutions.
# Solution 1:
# 
# 	0:  cost=0
# 
# 	ACTIONS:  destination(i)=table destination(k)=table move(i) move(k)
# 
# 	1:  cost=2
# 
# 	ACTIONS:  destination(c)=table destination(h)=table destination(j)=table move(c) move(h) move(j)
# 
# 	2:  cost=5
# 
# 	ACTIONS:  destination(e)=j destination(k)=g move(e) move(k)
# 
# 	3:  cost=7
# 
# 	ACTIONS:  destination(b)=h destination(c)=k destination(d)=table move(b) move(c) move(d)
# 
# 	4:  cost=10
# 
# 	ACTIONS:  destination(a)=e destination(b)=c destination(i)=d move(a) move(b) move(i)
# 
# 	5:  cost=13
# 
# 	ACTIONS:  destination(h)=i destination(l)=b destination(n)=a move(h) move(l) move(n)
# 
# 	6:  cost=16
# 
# 	ACTIONS:  destination(m)=h move(m)
# 
# 	7:  cost=17
# 
# 	ACTIONS:  destination(o)=m move(o)
# 
# 	8:  cost=18
# 
# 
# SATISFIABLE
# Models      : 1+
# Total Steps : 8
# Time        : 2.650
# Prepare   : 1.860
# Prepro.   : 0.700
# Solving   : 0.070


