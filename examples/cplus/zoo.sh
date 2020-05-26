# cplus2asp test script file.

######################################################################################################################
# Zoo World Tests
# --------------------------------------------------------------------------------------------------------------------

# ---------------------------------------------------- Zoo World --------------------------------------------------
echo "% ---------------------------------------------------- Zoo World --------------------------------------------------" 

# --------------------------- Test 1 ---------------------------
echo "% --------------------------- Test 1 ---------------------------" 
echo "$ cplus2asp zoo/zoo-test1.cp query=0 --shift" 
cplus2asp zoo/zoo-test1.cp query=0 --shift

# % Pragma: Running Query 'Query 0'.
# % Pragma: Minimum Step = '3', Maximum Step = '4'.
# % Pragma: Searching for 1 solutions.
# Solution 1:
# 
# 	0:  pos(homer)=6 pos(jumbo)=3
# 
# 	ACTIONS:  move(homer,7)
# 
# 	1:  pos(homer)=7 pos(jumbo)=3
# 
# 	ACTIONS:  open(homer,gateAO)
# 
# 	2:  opened(gateAO) pos(homer)=7 pos(jumbo)=3
# 
# 	ACTIONS:  move(homer,4)
# 
# 	3:  opened(gateAO) pos(homer)=4 pos(jumbo)=3
# 
# 	ACTIONS:  ab(homer) mount(homer,jumbo)
# 
# 	4:  mounted(homer,jumbo) opened(gateAO) pos(homer)=3 pos(jumbo)=3
# 
# 
# SATISFIABLE
# Models      : 1+
# Total Steps : 4
# Time        : 0.140
# Prepare   : 0.100
# Prepro.   : 0.040
# Solving   : 0.000


# --------------------------- Test 2 ---------------------------
echo "% --------------------------- Test 2 ---------------------------" 

echo "$ cplus2asp zoo/zoo-test2.cp query=0 --shift" 
cplus2asp zoo/zoo-test2.cp query=0 --shift

# % Pragma: Running Query 'Query 0'.
# % Pragma: Minimum Step = '2', Maximum Step = '2'.
# % Pragma: Searching for 1 solutions.
# Solution 1:
# 
# 	0:  pos(homer)=7
# 
# 	ACTIONS:  open(homer,gateAO)
# 
# 	1:  opened(gateAO) pos(homer)=7
# 
# 	ACTIONS:  move(homer,4)
# 
# 	2:  opened(gateAO) pos(homer)=4
# 
# 
# SATISFIABLE
# Models      : 1+
# Total Steps : 2
# Time        : 0.030
# Prepare   : 0.030
# Prepro.   : 0.000
# Solving   : 0.000


# --------------------------- Test 3 ---------------------------
echo "% --------------------------- Test 3 ---------------------------" 

echo "$ cplus2asp zoo/zoo-test3.cp query=0 --shift" 
cplus2asp zoo/zoo-test3.cp query=0 --shift

# % Pragma: Running Query 'Query 0'.
# % Pragma: Minimum Step = '1', Maximum Step = '2'.
# % Pragma: Searching for 1 solutions.
# Solution 1:
# 
# 	0:  pos(homer)=7 pos(snoopy)=4
# 
# 	ACTIONS:  open(homer,gateAO)
# 
# 	1:  opened(gateAO) pos(homer)=7 pos(snoopy)=4
# 
# 	ACTIONS:  move(homer,4) move(snoopy,7)
# 
# 	2:  opened(gateAO) pos(homer)=4 pos(snoopy)=7
# 
# 
# SATISFIABLE
# Models      : 1+
# Total Steps : 2
# Time        : 0.080
# Prepare   : 0.070
# Prepro.   : 0.010
# Solving   : 0.000



# --------------------------- Test 4 (Query 0) ---------------------------
echo "% --------------------------- Test 4 (Query 0) ---------------------------" 

echo "$ cplus2asp zoo/zoo-test4.cp query=0 --shift" 
cplus2asp zoo/zoo-test4.cp query=0 --shift


# % Pragma: Running Query 'Query 0'.
# % Pragma: Minimum Step = '3', Maximum Step = '10'.
# % Pragma: Searching for 1 solutions.
# Solution 1:
# 
# 	0:  pos(homer)=7 pos(jumbo)=1
# 
# 	ACTIONS:  move(homer,8) move(jumbo,4) open(homer,gateAO)
# 
# 	1:  opened(gateAO) pos(homer)=8 pos(jumbo)=4
# 
# 	ACTIONS:  move(homer,7) move(jumbo,7)
# 
# 	2:  opened(gateAO) pos(homer)=7 pos(jumbo)=7
# 
# 	ACTIONS:  move(homer,4) move(jumbo,8)
# 
# 	3:  opened(gateAO) pos(homer)=4 pos(jumbo)=8
# 
# 
# SATISFIABLE
# Models      : 1+
# Total Steps : 3
# Time        : 0.100
# Prepare   : 0.100
# Prepro.   : 0.000
# Solving   : 0.000

# --------------------------- Test 4 (Query 1) ---------------------------
echo "% --------------------------- Test 4 (Query 1) ---------------------------" 

echo "$ cplus2asp zoo/zoo-test4.cp query=1 --shift" 
cplus2asp zoo/zoo-test4.cp query=1 --shift

# % Pragma: Running Query 'Query 1'.
# % Pragma: Minimum Step = '4', Maximum Step = '10'.
# % Pragma: Searching for 1 solutions.
# Solution 1:
# 
# 	0:  pos(homer)=7 pos(jumbo)=1
# 
# 	ACTIONS:  move(jumbo,4) open(homer,gateAO)
# 
# 	1:  opened(gateAO) pos(homer)=7 pos(jumbo)=4
# 
# 	ACTIONS:  move(jumbo,7)
# 
# 	2:  opened(gateAO) pos(homer)=7 pos(jumbo)=7
# 
# 	ACTIONS:  move(homer,4) move(jumbo,8)
# 
# 	3:  opened(gateAO) pos(homer)=4 pos(jumbo)=8
# 
# 	ACTIONS:  move(homer,2)
# 
# 	4:  opened(gateAO) pos(homer)=2 pos(jumbo)=8
# 
# 
# SATISFIABLE
# Models      : 1+
# Total Steps : 4
# Time        : 0.140
# Prepare   : 0.110
# Prepro.   : 0.020
# Solving   : 0.000

# --------------------------- Test 5 (Query 0) ---------------------------
echo "% --------------------------- Test 5 (Query 0) ---------------------------" 

echo "$ cplus2asp zoo/zoo-test5.cp query=0 --shift" 
cplus2asp zoo/zoo-test5.cp query=0 --shift

# % Pragma: Running Query 'Query 0'.
# % Pragma: Minimum Step = '1', Maximum Step = '1'.
# % Pragma: Searching for 1 solutions.
# Solution 1:
# 
# 	0:  mounted(homer,silver) opened(gateAO) pos(homer)=4 pos(jumbo)=1 pos(silver)=4
# 
# 	ACTIONS:  ab(homer) move(jumbo,2) throwOff(silver,homer)
# 
# 	1:  opened(gateAO) pos(homer)=1 pos(jumbo)=2 pos(silver)=4
# 
# 
# SATISFIABLE
# Models      : 1+
# Time        : 0.080
# Prepare   : 0.070
# Prepro.   : 0.010
# Solving   : 0.000


# --------------------------- Test 5 (Query 1) ---------------------------
echo "% --------------------------- Test 5 (Query 1) ---------------------------" 

echo "$ cplus2asp zoo/zoo-test5.cp query=1 --shift" 
cplus2asp zoo/zoo-test5.cp query=1 --shift

# % Pragma: Running Query 'Query 1'.
# % Pragma: Minimum Step = '1', Maximum Step = '1'.
# % Pragma: Searching for 1 solutions.
# Solution 1:
# 
# 	0:  opened(gateAO) pos(homer)=2 pos(jumbo)=1 pos(silver)=1
# 
# 	ACTIONS:  ab(homer) mount(homer,jumbo) move(jumbo,3) move(silver,4)
# 
# 	1:  opened(gateAO) pos(homer)=1 pos(jumbo)=3 pos(silver)=4
# 
# 
# SATISFIABLE
# Models      : 1+
# Time        : 0.080
# Prepare   : 0.060
# Prepro.   : 0.020
# Solving   : 0.000

# --------------------------- Test 5 (Query 2) ---------------------------
echo "% --------------------------- Test 5 (Query 2) ---------------------------" 

echo "$ cplus2asp zoo/zoo-test5.cp query=2 --shift" 
cplus2asp zoo/zoo-test5.cp query=2 --shift

# % Pragma: Running Query 'Query 2'.
# % Pragma: Minimum Step = '1', Maximum Step = '1'.
# % Pragma: Searching for 1 solutions.
# % Pragma: No solutions found.
# No solution.
# 
# UNSATISFIABLE
# Models      : 0
# Time        : 0.080
# Prepare   : 0.060
# Prepro.   : 0.020
# Solving   : 0.000

# --------------------------- Test 5 (Query 11) ---------------------------
echo "% --------------------------- Test 5 (Query 11) ---------------------------" 

echo "$ cplus2asp zoo/zoo-test5.cp query=11 --shift" 
cplus2asp zoo/zoo-test5.cp query=11 --shift

# % Pragma: Running Query 'Query 11'.
# % Pragma: Minimum Step = '2', Maximum Step = '2'.
# % Pragma: Searching for 1 solutions.
# Solution 1:
# 
# 	0:  mounted(homer,jumbo) mounted(homer,silver) opened(gateAO) pos(homer)=8 pos(jumbo)=8 pos(silver)=8
# 
# 	1:  mounted(homer,jumbo) mounted(homer,silver) opened(gateAO) pos(homer)=8 pos(jumbo)=8 pos(silver)=8
# 
# 	ACTIONS:  ab(homer) move(silver,7) throwOff(silver,homer)
# 
# 	2:  mounted(homer,jumbo) opened(gateAO) pos(homer)=8 pos(jumbo)=8 pos(silver)=7
# 
# 
# SATISFIABLE
# Models      : 1+
# Total Steps : 2
# Time        : 0.130
# Prepare   : 0.100
# Prepro.   : 0.030
# Solving   : 0.000

# --------------------------- Test 6 ---------------------------
echo "% --------------------------- Test 6 ---------------------------" 

echo "$ cplus2asp zoo/zoo-test6.cp query=0 --shift" 
cplus2asp zoo/zoo-test6.cp query=0 --shift

# % Pragma: Running Query 'Query 0'.
# % Pragma: Minimum Step = '1', Maximum Step = '1'.
# % Pragma: Searching for 1 solutions.
# Solution 1:
# 
# 	0:  mounted(homer,jumbo) opened(gateAO) pos(homer)=1 pos(jumbo)=1
# 
# 	ACTIONS:  ab(homer) move(jumbo,4) throwOff(jumbo,homer)
# 
# 	1:  opened(gateAO) pos(homer)=3 pos(jumbo)=4
# 
# 
# SATISFIABLE
# Models      : 1+
# Time        : 0.050
# Prepare   : 0.040
# Prepro.   : 0.010
# Solving   : 0.000
