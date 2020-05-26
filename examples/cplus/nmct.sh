# cplus2asp3 test script file.

######################################################################################################################
# NMCT Domains
# --------------------------------------------------------------------------------------------------------------------

# ------------------------------------------------ Monkeys & Bananas -------------------------------------------------
echo "% ------------------------------------------------ Monkeys & Bananas -------------------------------------------------" 

# ------------------------- Query 0 -------------------------
echo "% ------------------------- Query 0 -------------------------" 

echo "$ cplus2asp3 nmct/monkey query=0 0 " 
cplus2asp3 nmct/monkey query=0 0 

# Output:

# % Pragma: Running Query 'Query 0'.
# % Pragma: Minimum Step = '0', Maximum Step = '2'.
# % Pragma: Searching for all solutions.
# % Pragma: No solutions found.
# No solution.
# 
# UNSATISFIABLE
# Models      : 0
# Total Steps : 2
# Time        : 0.000
# Prepare   : 0.000
# Prepro.   : 0.000
# Solving   : 0.000

# ------------------------- Query 1 -------------------------
echo "% ------------------------- Query 1 -------------------------" 

echo "$ cplus2asp3 nmct/monkey query=1 0 " 
cplus2asp3 nmct/monkey query=1 0 

# Output:

# % Pragma: Running Query 'Query 1'.
# % Pragma: Minimum Step = '0', Maximum Step = '2'.
# % Pragma: Searching for all solutions.
# % Pragma: No solutions found.
# No solution.

# UNSATISFIABLE
# Models      : 0
# Total Steps : 2
# Time        : 0.000
# Prepare   : 0.000
# Prepro.   : 0.000
# Solving   : 0.000

# ------------------------- Query 2 -------------------------
echo "% ------------------------- Query 2 -------------------------" 

echo "$ cplus2asp3 nmct/monkey query=2 0 " 
cplus2asp3 nmct/monkey query=2 0 

# Output:

# % Pragma: Running Query 'Query 2'.
# % Pragma: Minimum Step = '1', Maximum Step = '100'.
# % Pragma: Searching for all solutions.
# Solution 1:
#
#	0:  loc(bananas)=l2 loc(box)=l3 loc(monkey)=l1
#
#	ACTIONS:  walk(l3)
#
#	1:  loc(bananas)=l2 loc(box)=l3 loc(monkey)=l3
#
#	ACTIONS:  pushBox(l2)
#
#	2:  loc(bananas)=l2 loc(box)=l2 loc(monkey)=l2
#
#	ACTIONS:  climbOn
#
#	3:  loc(bananas)=l2 loc(box)=l2 loc(monkey)=l2 onBox
#
#	ACTIONS:  graspBananas
#
#	4:  hasBananas loc(bananas)=l2 loc(box)=l2 loc(monkey)=l2 onBox
#
#
# SATISFIABLE
# Models      : 1
# Total Steps : 4
# Time        : 0.000
# Prepare   : 0.000
# Prepro.   : 0.000
# Solving   : 0.000

# ----------------------------------------------------- Shooting -----------------------------------------------------
echo "% ----------------------------------------------------- Shooting -----------------------------------------------------" 

# ------------------------- Query 0 -------------------------
echo "% ------------------------- Query 0 -------------------------" 

echo "$ cplus2asp3 nmct/shooting query=0 0 " 
cplus2asp3 nmct/shooting query=0 0 

# Output:

# % Pragma: Running Query 'Query 0'.
# % Pragma: Minimum Step = '0', Maximum Step = '6'.
# % Pragma: Searching for all solutions.
# Solution 1:
# 
# 	0:  alive(turkey1) alive(turkey2)
# 
# 	ACTIONS:  load
# 
# 	1:  alive(turkey1) alive(turkey2) loaded
# 
# 	ACTIONS:  aim(turkey1)
# 
# 	2:  alive(turkey1) alive(turkey2) loaded target=turkey1
# 
# 	ACTIONS:  shoot
# 
# 	3:  alive(turkey2) target=turkey1
# 
# 	ACTIONS:  load
# 
# 	4:  alive(turkey2) loaded
# 
# 	ACTIONS:  aim(turkey2)
# 
# 	5:  alive(turkey2) loaded target=turkey2
# 
# 	ACTIONS:  shoot
# 
# 	6:  target=turkey2
# 
# 
# Solution 2:
# 
# 	0:  alive(turkey1) alive(turkey2)
# 
# 	ACTIONS:  load
# 
# 	1:  alive(turkey1) alive(turkey2) loaded
# 
# 	ACTIONS:  aim(turkey2)
# 
# 	2:  alive(turkey1) alive(turkey2) loaded target=turkey2
# 
# 	ACTIONS:  shoot
# 
# 	3:  alive(turkey1) target=turkey2
# 
# 	ACTIONS:  load
# 
# 	4:  alive(turkey1) loaded
# 
# 	ACTIONS:  aim(turkey1)
# 
# 	5:  alive(turkey1) loaded target=turkey1
# 
# 	ACTIONS:  shoot
# 
# 	6:  target=turkey1
# 
# 
# SATISFIABLE
# Models      : 2
# Total Steps : 6
# Time        : 0.000
# Prepare   : 0.000
# Prepro.   : 0.000
# Solving   : 0.000

# ------------------------------------------------------ Going -------------------------------------------------------
echo "% ------------------------------------------------------ Going -------------------------------------------------------" 

# --------------------------- Query 0 ---------------------------
echo "% --------------------------- Query 0 ---------------------------" 

echo "$ cplus2asp3 nmct/going query=0 0" 
cplus2asp3 nmct/going query=0 0

# % Pragma: Running Query 'Query 0'.
# % Pragma: Minimum Step = '1', Maximum Step = '1'.
# % Pragma: Searching for all solutions.
# Solution 1:
# 
# 	0:  loc(car)=work loc(jack)=home
# 
# 	ACTIONS:  go(work)
# 
# 	1:  loc(car)=work loc(jack)=work
# 
# 
# Solution 2:
# 
# 	0:  loc(car)=home loc(jack)=home
# 
# 	ACTIONS:  go(work)
# 
# 	1:  loc(car)=home loc(jack)=work
# 
# 
# Solution 3:
# 
# 	0:  loc(car)=home loc(jack)=home
# 
# 	ACTIONS:  go(work)
# 
# 	1:  loc(car)=work loc(jack)=work
# 
# 
# SATISFIABLE
# Models      : 3
# Time        : 0.000
# Prepare   : 0.000
# Prepro.   : 0.000
# Solving   : 0.000

# --------------------------- Query 1 ---------------------------
echo "% --------------------------- Query 1 ---------------------------" 

echo "$ cplus2asp3 nmct/going query=1 0" 
cplus2asp3 nmct/going query=1 0

# % Pragma: Running Query 'Query 1'.
# % Pragma: Minimum Step = '0', Maximum Step = '0'.
# % Pragma: Searching for all solutions.
# Solution 1:
# 
# 	0:  loc(car)=work loc(jack)=work
# 
# 
# Solution 2:
# 
# 	0:  loc(car)=work loc(jack)=home
# 
# 
# Solution 3:
# 
# 	0:  loc(car)=home loc(jack)=work
# 
# 
# Solution 4:
# 
# 	0:  loc(car)=home loc(jack)=home
# 
# 
# SATISFIABLE
# Models      : 4
# Time        : 0.000
# Prepare   : 0.000
# Prepro.   : 0.000
# Solving   : 0.000

# --------------------------- Query 2 ---------------------------
echo "% --------------------------- Query 2 ---------------------------" 

echo "$ cplus2asp3 nmct/going query=2 0" 
cplus2asp3 nmct/going query=2 0

# % Pragma: Running Query 'Query 2'.
# % Pragma: Minimum Step = '1', Maximum Step = '1'.
# % Pragma: Searching for all solutions.
# Solution 1:
# 
# 	0:  loc(car)=home loc(jack)=home
# 
# 	ACTIONS:  go(work)
# 
# 	1:  loc(car)=home loc(jack)=work
# 
# 
# Solution 2:
# 
# 	0:  loc(car)=home loc(jack)=home
# 
# 	ACTIONS:  go(work)
# 
# 	1:  loc(car)=work loc(jack)=work
# 
# 
# SATISFIABLE
# Models      : 2
# Time        : 0.000
# Prepare   : 0.000
# Prepro.   : 0.000
# Solving   : 0.000




# ----------------------------------------------------- Lifting ------------------------------------------------------
echo "% ----------------------------------------------------- Lifting ------------------------------------------------------" 

# ------------------------- Query 0 -------------------------
echo "% ------------------------- Query 0 -------------------------" 

echo "$ cplus2asp3 nmct/lifting query=0 0 " 
cplus2asp3 nmct/lifting query=0 0 


# Output:

# % Pragma: Running Query 'Query 0'.
# % Pragma: Minimum Step = '0', Maximum Step = '1'.
# % Pragma: Searching for all solutions.
# Solution 1:
# 
# 	0:  level(leftend)=high level(rightend)=high onTable
# 
# 	1:  level(leftend)=high level(rightend)=high onTable
# 
# 
# Solution 2:
# 
# 	0:  level(leftend)=low level(rightend)=low onTable
# 
# 	1:  level(leftend)=low level(rightend)=low onTable
# 
# 
# Solution 3:
# 
# 	0:  level(leftend)=low level(rightend)=low onTable
# 
# 	ACTIONS:  lift(leftend) lift(rightend)
# 
# 	1:  level(leftend)=high level(rightend)=high onTable
# 
# 
# SATISFIABLE
# Models      : 3
# Time        : 0.000
# Prepare   : 0.000
# Prepro.   : 0.000
# Solving   : 0.000

# ----------------------------------------------------- Pendulum -----------------------------------------------------
echo "% ----------------------------------------------------- Pendulum -----------------------------------------------------" 

# ------------------------- Query 0 -------------------------
echo "% ------------------------- Query 0 -------------------------" 

echo "$ cplus2asp3 nmct/pendulum query=0 0 " 
cplus2asp3 nmct/pendulum query=0 0 

# Output:

# % Pragma: Running Query 'Query 0'.
# % Pragma: Minimum Step = '2', Maximum Step = '2'.
# % Pragma: Searching for all solutions.
# Solution 1:
# 
# 	0: 
# 
# 	ACTIONS:  hold
# 
# 	1: 
# 	2:  right
# 
# 
# Solution 2:
# 
# 	0: 
# 
# 	ACTIONS:  hold
# 
# 	1: 
# 
# 	ACTIONS:  hold
# 
# 	2: 
# 
# Solution 3:
# 
# 	0: 
# 	1:  right
# 
# 	2: 
# 
# Solution 4:
# 
# 	0: 
# 	1:  right
# 
# 	ACTIONS:  hold
# 
# 	2:  right
# 
# 
# SATISFIABLE
# Models      : 4
# Total Steps : 2
# Time        : 0.000
# Prepare   : 0.000
# Prepro.   : 0.000
# Solving   : 0.000

# ---------------------------------------------------- Publishing ----------------------------------------------------
echo "% ---------------------------------------------------- Publishing ----------------------------------------------------" 

# ------------------------- Query 0 -------------------------
echo "% ------------------------- Query 0 -------------------------" 

echo "$ cplus2asp3 nmct/publishing query=0 " 
cplus2asp3 nmct/publishing query=0 

# Output:

# % Pragma: Running Query 'Query 0'.
# % Pragma: Minimum Step = '0', Maximum Step = '3'.
# % Pragma: Searching for 1 solutions.
# Solution 1:
# 
# 	0: 
# 
# 	ACTIONS:  kind=conference length=10 publish
# 
# 	1:  hasPublications
# 
# 	ACTIONS:  kind=conference length=40 publish
# 
# 	2:  hasLongPublications hasPublications
# 
# 	ACTIONS:  kind=journal length=71 publish
# 
# 	3:  hasJournalPublications hasLongPublications hasPublications
# 
# 
# SATISFIABLE
# Models      : 1+
# Total Steps : 3
# Time        : 0.010
# Prepare   : 0.010
# Prepro.   : 0.000
# Solving   : 0.000



