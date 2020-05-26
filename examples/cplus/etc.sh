# cplus2asp test script file.

######################################################################################################################
# Misc Domains
# --------------------------------------------------------------------------------------------------------------------

# ------------------------------------------------------ Clique ------------------------------------------------------
echo "% ------------------------------------------------------ Clique ------------------------------------------------------" 

# --------------------------- states ---------------------------
echo "% --------------------------- states ---------------------------" 

echo "$ cplus2asp etc/clique query=states --shift 0" 
cplus2asp etc/clique query=states --shift 0

#Output:

# % Pragma: Running Query 'Satisfiability Check'.
# % Pragma: Minimum Step = '0', Maximum Step = '0'.
# % Pragma: Searching for all solutions.
# Solution 1:
# 
# 	RIGID CONSTANTS:  member(1)=1 member(2)=3 member(3)=4
# 
# 
# Solution 2:
# 
# 	RIGID CONSTANTS:  member(1)=0 member(2)=1 member(3)=2
# 
# 
# SATISFIABLE
# Models      : 2
# Time        : 0.010
# Prepare   : 0.010
# Prepro.   : 0.000
# Solving   : 0.000



# ------------------------------------------------------ Color ------------------------------------------------------
echo "% ------------------------------------------------------ Color ------------------------------------------------------" 

# --------------------------- states ---------------------------
echo "% --------------------------- states ---------------------------" 

echo "$ cplus2asp etc/color query=states" 
cplus2asp etc/color query=states

# % Pragma: Running Query 'Satisfiability Check'.
# % Pragma: Minimum Step = '0', Maximum Step = '0'.
# % Pragma: Searching for 1 solutions.
# Solution 1:
# 
# 	RIGID CONSTANTS:  col(0)=2 col(1)=3 col(2)=1 col(3)=2 col(4)=1 col(5)=3 col(6)=2
# 
# 
# SATISFIABLE
# Models      : 1+
# Time        : 0.000
# Prepare   : 0.000
# Prepro.   : 0.000
# Solving   : 0.000




# ------------------------------------------------------ Hanoi ------------------------------------------------------
echo "% ------------------------------------------------------ Hanoi ------------------------------------------------------" 

# --------------------------- Query 0 ---------------------------
echo "% --------------------------- Query 0 ---------------------------" 

echo "$ cplus2asp etc/hanoi query=0" 
cplus2asp etc/hanoi query=0

# % Pragma: Running Query 'Query 0'.
# % Pragma: Minimum Step = '6', Maximum Step = '7'.
# % Pragma: Searching for 1 solutions.
# Solution 1:
# 
# 	RIGID CONSTANTS:  size(d1)=2 size(d2)=1 size(d3)=0 size(table)=3
# 
# 	0:  loc(d1)=table loc(d2)=d1 loc(d3)=d2 top(p1)=d3 top(p2)=table top(p3)=table
# 
# 	ACTIONS:  move(p1,p3)
# 
# 	1:  loc(d1)=table loc(d2)=d1 loc(d3)=table top(p1)=d2 top(p2)=table top(p3)=d3
# 
# 	ACTIONS:  move(p1,p2)
# 
# 	2:  loc(d1)=table loc(d2)=table loc(d3)=table top(p1)=d1 top(p2)=d2 top(p3)=d3
# 
# 	ACTIONS:  move(p3,p2)
# 
# 	3:  loc(d1)=table loc(d2)=table loc(d3)=d2 top(p1)=d1 top(p2)=d3 top(p3)=table
# 
# 	ACTIONS:  move(p1,p3)
# 
# 	4:  loc(d1)=table loc(d2)=table loc(d3)=d2 top(p1)=table top(p2)=d3 top(p3)=d1
# 
# 	ACTIONS:  move(p2,p1)
# 
# 	5:  loc(d1)=table loc(d2)=table loc(d3)=table top(p1)=d3 top(p2)=d2 top(p3)=d1
# 
# 	ACTIONS:  move(p2,p3)
# 
# 	6:  loc(d1)=table loc(d2)=d1 loc(d3)=table top(p1)=d3 top(p2)=table top(p3)=d2
# 
# 	ACTIONS:  move(p1,p3)
# 
# 	7:  loc(d1)=table loc(d2)=d1 loc(d3)=d2 top(p1)=table top(p2)=table top(p3)=d3
# 
# 
# SATISFIABLE
# Models      : 1+
# Total Steps : 7
# Time        : 0.040
# Prepare   : 0.030
# Prepro.   : 0.000
# Solving   : 0.010

# --------------------------- Query 1 ---------------------------
echo "% --------------------------- Query 1 ---------------------------" 

echo "$ cplus2asp etc/hanoi query=1" 
cplus2asp etc/hanoi query=1

# % Pragma: Running Query 'Query 1'.
# % Pragma: Minimum Step = '0', Maximum Step = '0'.
# % Pragma: Searching for 1 solutions.
# % Pragma: No solutions found.
# No solution.
# 
# UNSATISFIABLE
# Models      : 0
# Time        : 0.000
# Prepare   : 0.000
# Prepro.   : 0.000
# Solving   : 0.000


# ------------------------------------------------------ Hanoi2 -----------------------------------------------------
echo "% ------------------------------------------------------ Hanoi2 -----------------------------------------------------" 

# --------------------------- Query 0 ---------------------------
echo "% --------------------------- Query 0 ---------------------------" 

echo "$ cplus2asp etc/hanoi2 query=0 0 --none-hack=false" 
cplus2asp etc/hanoi2 query=0 0 --none-hack=false

# % Pragma: Running Query 'Query 0'.
# % Pragma: Minimum Step = '7', Maximum Step = '7'.
# % Pragma: Searching for all solutions.
# Solution 1:
# 
# 	0:  loc(d(2))=d(1) loc(d(3))=d(2) size(d(1))=2 size(d(2))=1 size(d(3))=0 size(none)=3 top(p(1))=d(3)
# 
# 	ACTIONS:  move(p(1),p(3))
# 
# 	1:  loc(d(2))=d(1) size(d(1))=2 size(d(2))=1 size(d(3))=0 size(none)=3 top(p(1))=d(2) top(p(3))=d(3)
# 
# 	ACTIONS:  move(p(1),p(2))
# 
# 	2:  size(d(1))=2 size(d(2))=1 size(d(3))=0 size(none)=3 top(p(1))=d(1) top(p(2))=d(2) top(p(3))=d(3)
# 
# 	ACTIONS:  move(p(3),p(2))
# 
# 	3:  loc(d(3))=d(2) size(d(1))=2 size(d(2))=1 size(d(3))=0 size(none)=3 top(p(1))=d(1) top(p(2))=d(3)
# 
# 	ACTIONS:  move(p(1),p(3))
# 
# 	4:  loc(d(3))=d(2) size(d(1))=2 size(d(2))=1 size(d(3))=0 size(none)=3 top(p(2))=d(3) top(p(3))=d(1)
# 
# 	ACTIONS:  move(p(2),p(1))
# 
# 	5:  size(d(1))=2 size(d(2))=1 size(d(3))=0 size(none)=3 top(p(1))=d(3) top(p(2))=d(2) top(p(3))=d(1)
# 
# 	ACTIONS:  move(p(2),p(3))
# 
# 	6:  loc(d(2))=d(1) size(d(1))=2 size(d(2))=1 size(d(3))=0 size(none)=3 top(p(1))=d(3) top(p(3))=d(2)
# 
# 	ACTIONS:  move(p(1),p(3))
# 
# 	7:  loc(d(2))=d(1) loc(d(3))=d(2) size(d(1))=2 size(d(2))=1 size(d(3))=0 size(none)=3 top(p(3))=d(3)
# 
# 
# SATISFIABLE
# Models      : 1
# Total Steps : 7
# Time        : 0.060
# Prepare   : 0.020
# Prepro.   : 0.010
# Solving   : 0.030


# --------------------------- Query 1 ---------------------------
echo "% --------------------------- Query 1 ---------------------------" 

echo "$ cplus2asp etc/hanoi2 query=1 0 --none-hack=false" 
cplus2asp etc/hanoi2 query=1 0 --none-hack=false


# % Pragma: Running Query 'Query 1'.
# % Pragma: Minimum Step = '1', Maximum Step = '1'.
# % Pragma: Searching for 1 solutions.
# % Pragma: No solutions found.
# No solution.
# 
# UNSATISFIABLE
# Models      : 0
# Time        : 0.000
# Prepare   : 0.000
# Prepro.   : 0.000
# Solving   : 0.000


# ------------------------------------------------------ Has -----------------------------------------------------
echo "% ------------------------------------------------------ Has -----------------------------------------------------" 

# --------------------------- Query 0 ---------------------------
echo "% --------------------------- Query 0 ---------------------------" 
echo "$ cplus2asp etc/has query=0 0 --none-hack=false" 
cplus2asp etc/has query=0 0 --none-hack=false


# % Pragma: Running Query 'Query 0'.
# % Pragma: Minimum Step = '3', Maximum Step = '3'.
# % Pragma: Searching for all solutions.
# Solution 3:
# 
# 	0:  has=0
# 
# 	ACTIONS:  buy
# 
# 	1:  has=1
# 
# 	ACTIONS:  buy
# 
# 	2:  has=2
# 
# 	3:  has=2
# 
# 
# Solution 4:
# 
# 	0:  has=1
# 
# 	ACTIONS:  buy
# 
# 	1:  has=2
# 
# 	ACTIONS:  buy
# 
# 	2:  has=3
# 
# 	3:  has=3
# 
# 
# Solution 5:
# 
# 	0:  has=0
# 
# 	ACTIONS:  buy
# 
# 	1:  has=1
# 
# 	ACTIONS:  buy
# 
# 	2:  has=2
# 
# 	ACTIONS:  buy
# 
# 	3:  has=3
# 
# 
# SATISFIABLE
# Models      : 3
# Total Steps : 3
# Time        : 0.000
# Prepare   : 0.000
# Prepro.   : 0.000
# Solving   : 0.000


# ---------------------------------------------------- Multiple Monkeys --------------------------------------------------
echo "% ---------------------------------------------------- Multiple Monkeys --------------------------------------------------" 

# --------------------------- Query 0 ---------------------------
echo "% --------------------------- Query 0 ---------------------------" 

echo "$ cplus2asp etc/multiple-monkey-test query=0" 
cplus2asp etc/multiple-monkey-test query=0

# % Pragma: Running Query 'Query 0'.
# % Pragma: Minimum Step = '0', Maximum Step = '0'.
# % Pragma: Searching for 1 solutions.
# Solution 1:
# 
# 	0:  ab1(alice,david) ab1(david,alice) ab1(david,david) ab1(david,goliath) ab1(goliath,david) hasBananas(goliath) loc(alice)=l3 loc(bananas)=l3 loc(box)=l3 loc(david)=l3 loc(goliath)=l3 onBox(alice) onBox(david)
# 
# 
# SATISFIABLE
# Models      : 1+
# Time        : 0.000
# Prepare   : 0.000
# Prepro.   : 0.000
# Solving   : 0.000


# --------------------------- Query 1 ---------------------------
echo "% --------------------------- Query 1 ---------------------------" 

echo "$ cplus2asp etc/multiple-monkey-test query=1" 
cplus2asp etc/multiple-monkey-test query=1

# % Pragma: Running Query 'Query 1'.
# % Pragma: Minimum Step = '1', Maximum Step = '1'.
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


# --------------------------- Query 2 ---------------------------
echo "% --------------------------- Query 2 ---------------------------" 

echo "$ cplus2asp etc/multiple-monkey-test query=2" 
cplus2asp etc/multiple-monkey-test query=2

# % Pragma: Running Query 'Query 2'.
# % Pragma: Minimum Step = '1', Maximum Step = '1'.
# % Pragma: Searching for 1 solutions.
# Solution 1:
# 
# 	0:  ab1(alice,david) ab1(david,alice) ab1(david,david) ab1(david,goliath) ab1(goliath,david) hasBananas(goliath) loc(alice)=l1 loc(bananas)=l1 loc(box)=l1 loc(david)=l1 loc(goliath)=l1 onBox(david) onBox(goliath)
# 
# 	ACTIONS:  ab2(alice) ab2(david) ab2(goliath) pushBox(alice,l3)
# 
# 	1:  ab1(alice,david) ab1(david,alice) ab1(david,david) ab1(david,goliath) ab1(goliath,david) hasBananas(goliath) loc(alice)=l1 loc(bananas)=l1 loc(box)=l1 loc(david)=l1 loc(goliath)=l1 onBox(david) onBox(goliath)
# 
# 
# SATISFIABLE
# Models      : 1+
# Time        : 0.010
# Prepare   : 0.000
# Prepro.   : 0.000
# Solving   : 0.010

# --------------------------- Query 3 ---------------------------
echo "% --------------------------- Query 3 ---------------------------" 

echo "$ cplus2asp etc/multiple-monkey-test query=3"
cplus2asp etc/multiple-monkey-test query=3

# % Pragma: Running Query 'Query 3'.
# % Pragma: Minimum Step = '1', Maximum Step = '1'.
# % Pragma: Searching for 1 solutions.
# Solution 1:
# 
# 	0:  ab1(alice,david) ab1(david,alice) ab1(david,david) ab1(david,goliath) ab1(goliath,david) hasBananas(goliath) loc(alice)=l1 loc(bananas)=l1 loc(box)=l1 loc(david)=l1 loc(goliath)=l1
# 
# 	ACTIONS:  climbOn(david) pushBox(alice,l2) pushBox(goliath,l2)
# 
# 	1:  ab1(alice,david) ab1(david,alice) ab1(david,david) ab1(david,goliath) ab1(goliath,david) hasBananas(goliath) loc(alice)=l2 loc(bananas)=l2 loc(box)=l2 loc(david)=l2 loc(goliath)=l2 onBox(david)
# 
# 
# SATISFIABLE
# Models      : 1+
# Time        : 0.000
# Prepare   : 0.000
# Prepro.   : 0.000
# Solving   : 0.000


# --------------------------- Query 4 ---------------------------
echo "% --------------------------- Query 4 ---------------------------" 

echo "$ cplus2asp etc/multiple-monkey-test query=4"
cplus2asp etc/multiple-monkey-test query=4

# % Pragma: Running Query 'Query 4'.
# % Pragma: Minimum Step = '1', Maximum Step = '1'.
# % Pragma: Searching for 1 solutions.
# % Pragma: No solutions found.
# No solution.
# 
# UNSATISFIABLE
# Models      : 0
# Time        : 0.000
# Prepare   : 0.000
# Prepro.   : 0.000
# Solving   : 0.000


# --------------------------- Query 5 ---------------------------
echo "% --------------------------- Query 5 ---------------------------" 

echo "$ cplus2asp etc/multiple-monkey-test query=5"
cplus2asp etc/multiple-monkey-test query=5

# % Pragma: Running Query 'Query 5'.
# % Pragma: Minimum Step = '1', Maximum Step = '1'.
# % Pragma: Searching for 1 solutions.
# Solution 1:
# 
# 	0:  ab1(alice,david) ab1(david,alice) ab1(david,david) ab1(david,goliath) ab1(goliath,david) hasBananas(goliath) loc(alice)=l1 loc(bananas)=l1 loc(box)=l1 loc(david)=l1 loc(goliath)=l1 onBox(david)
# 
# 	ACTIONS:  climbOff(david) pushBox(alice,l2) pushBox(goliath,l2)
# 
# 	1:  ab1(alice,david) ab1(david,alice) ab1(david,david) ab1(david,goliath) ab1(goliath,david) hasBananas(goliath) loc(alice)=l2 loc(bananas)=l2 loc(box)=l2 loc(david)=l1 loc(goliath)=l2
# 
# 
# SATISFIABLE
# Models      : 1+
# Time        : 0.000
# Prepare   : 0.000
# Prepro.   : 0.000
# Solving   : 0.000

# --------------------------- Query 6 ---------------------------
echo "% --------------------------- Query 6 ---------------------------" 

echo "$ cplus2asp etc/multiple-monkey-test query=6"
cplus2asp etc/multiple-monkey-test query=6

# % Pragma: Running Query 'Query 6'.
# % Pragma: Minimum Step = '1', Maximum Step = '1'.
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


# --------------------------- Query 7 ---------------------------
echo "% --------------------------- Query 7 ---------------------------" 

echo "$ cplus2asp etc/multiple-monkey-test query=7"
cplus2asp etc/multiple-monkey-test query=7

# % Pragma: Running Query 'Query 7'.
# % Pragma: Minimum Step = '1', Maximum Step = '1'.
# % Pragma: Searching for 1 solutions.
# Solution 1:
# 
# 	0:  ab1(alice,david) ab1(david,alice) ab1(david,david) ab1(david,goliath) ab1(goliath,david) hasBananas(david) loc(alice)=l1 loc(bananas)=l1 loc(box)=l1 loc(david)=l1 loc(goliath)=l3
# 
# 	ACTIONS:  climbOn(david) pushBox(alice,l2) walk(david,l2)
# 
# 	1:  ab1(alice,david) ab1(david,alice) ab1(david,david) ab1(david,goliath) ab1(goliath,david) hasBananas(david) loc(alice)=l2 loc(bananas)=l2 loc(box)=l2 loc(david)=l2 loc(goliath)=l3 onBox(david)
# 
# 
# SATISFIABLE
# Models      : 1+
# Time        : 0.010
# Prepare   : 0.010
# Prepro.   : 0.000
# Solving   : 0.000


# ---------------------------------------------------- 8 Queens --------------------------------------------------
echo "% ---------------------------------------------------- 8 Queens --------------------------------------------------" 

# --------------------------- states ---------------------------
echo "% --------------------------- states ---------------------------" 

echo "$ cplus2asp etc/queens query=states"
cplus2asp etc/queens query=states

# % Pragma: Running Query 'Satisfiability Check'.
# % Pragma: Minimum Step = '0', Maximum Step = '0'.
# % Pragma: Searching for 1 solutions.
# Solution 1:
# 
# 	RIGID CONSTANTS:  queen(1)=4 queen(2)=7 queen(3)=5 queen(4)=2 queen(5)=6 queen(6)=1 queen(7)=3 queen(8)=8
# 
# 
# SATISFIABLE
# Models      : 1+
# Time        : 0.020
# Prepare   : 0.010
# Prepro.   : 0.000
# Solving   : 0.010


# ---------------------------------------------------- Snack --------------------------------------------------
echo "% ---------------------------------------------------- Snack --------------------------------------------------" 

# --------------------------- Query 0 ---------------------------
echo "% --------------------------- Query 0 ---------------------------" 

echo "$ cplus2asp etc/snack query=0 0"
cplus2asp etc/snack query=0 0


# % Pragma: Running Query 'Query 0'.
# % Pragma: Minimum Step = '2', Maximum Step = '2'.
# % Pragma: Searching for all solutions.
# Solution 1:
# 
# 	0:  owner(apple)=i owner(banana)=i owner(carrot)=i
# 
# 	ACTIONS:  fightFor(left,apple)
# 
# 	1:  owner(apple)=i owner(banana)=i owner(carrot)=i
# 
# 	ACTIONS:  fightFor(right,banana)
# 
# 	2:  owner(apple)=i owner(banana)=i owner(carrot)=i
# 
# 
# Solution 2:
# 
# 	0:  owner(apple)=i owner(banana)=i owner(carrot)=i
# 
# 	ACTIONS:  fightFor(left,apple)
# 
# 	1:  owner(apple)=left owner(banana)=i owner(carrot)=i
# 
# 	ACTIONS:  fightFor(right,banana)
# 
# 	2:  owner(apple)=left owner(banana)=i owner(carrot)=i
# 
# 
# Solution 3:
# 
# 	0:  owner(apple)=i owner(banana)=i owner(carrot)=i
# 
# 	ACTIONS:  fightFor(left,apple)
# 
# 	1:  owner(apple)=i owner(banana)=i owner(carrot)=i
# 
# 	ACTIONS:  fightFor(right,banana)
# 
# 	2:  owner(apple)=i owner(banana)=right owner(carrot)=i
# 
# 
# Solution 4:
# 
# 	0:  owner(apple)=i owner(banana)=i owner(carrot)=i
# 
# 	ACTIONS:  fightFor(left,apple)
# 
# 	1:  owner(apple)=left owner(banana)=i owner(carrot)=i
# 
# 	ACTIONS:  fightFor(right,banana)
# 
# 	2:  owner(apple)=left owner(banana)=right owner(carrot)=i
# 
# 
# SATISFIABLE
# Models      : 4
# Total Steps : 2
# Time        : 0.000
# Prepare   : 0.000
# Prepro.   : 0.000
# Solving   : 0.000



