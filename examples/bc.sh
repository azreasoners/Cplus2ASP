# cplus2asp test script file.

######################################################################################################################
# BC Examples
# --------------------------------------------------------------------------------------------------------------------

# ----------------------------------------------------- BC Examples ---------------------------------------------------
echo "% ------------------------------------------------------ BC Examples ----------------------------------------------------" 



# --------------------------- Blocks World ---------------------------
echo "% --------------------------- Blocks World ---------------------------" 
echo "$ cplus2asp bc/bw.bc -l bc query=stack" 
cplus2asp bc/bw.bc -l bc query=stack

# % Pragma: Running Query 'Query 'stack''.
# % Pragma: Minimum Step = '0', No Maximum Step.
# % Pragma: Searching for 1 solutions.
# Solution: 1
#         0:  in_tower(b1) in_tower(b2) in_tower(b3) in_tower(b4) loc(b1)=table loc(b2)=table loc(b3)=table loc(b4)=table
# 
#         ACTIONS:  move(b2,b1) move(b3,b2) move(b4,b3)
# 
#         1:  in_tower(b1) in_tower(b2) in_tower(b3) in_tower(b4) loc(b1)=table loc(b2)=b1 loc(b3)=b2 loc(b4)=b3
# 
# SATISFIABLE
# Models      : 1+
# Time        : 0.000
# Prepare   : 0.000
# Prepro.   : 0.000


# --------------------------- Blocks World (Alt) ---------------------------
echo "% --------------------------- Blocks World (Alt) ---------------------------" 
echo "$ cplus2asp bc/bw-alt.bc -l bc query=stack" 
cplus2asp bc/bw-alt.bc -l bc query=stack


# % Pragma: Running Query 'Query 'stack''.
# % Pragma: Minimum Step = '0', No Maximum Step.
# % Pragma: Searching for 1 solutions.
# Solution: 1
#         0:  on(blk(1))=tbl(1) on(blk(2))=tbl(2) on(blk(3))=tbl(3) on(blk(4))=tbl(4) pos(blk(1))=tbl(1) pos(blk(2))=tbl(2) pos(blk(3))=tbl(3) pos(blk(4))=tbl(4) pos(tbl(1))=tbl(1) pos(tbl(2))=tbl(2) pos(tbl(3))=tbl(3) pos(tbl(4))=tbl(4) pos(tbl(5))=tbl(5)
# 
#         ACTIONS:  move(left,blk(3),blk(2)) move(right,blk(4),tbl(5))
# 
#         1:  on(blk(1))=tbl(1) on(blk(2))=tbl(2) on(blk(3))=blk(2) on(blk(4))=tbl(5) pos(blk(1))=tbl(1) pos(blk(2))=tbl(2) pos(blk(3))=tbl(2) pos(blk(4))=tbl(5) pos(tbl(1))=tbl(1) pos(tbl(2))=tbl(2) pos(tbl(3))=tbl(3) pos(tbl(4))=tbl(4) pos(tbl(5))=tbl(5)
# 
#         ACTIONS:  move(left,blk(1),blk(3)) move(right,blk(4),blk(1))
# 
#         2:  on(blk(1))=blk(3) on(blk(2))=tbl(2) on(blk(3))=blk(2) on(blk(4))=blk(1) pos(blk(1))=tbl(2) pos(blk(2))=tbl(2) pos(blk(3))=tbl(2) pos(blk(4))=tbl(2) pos(tbl(1))=tbl(1) pos(tbl(2))=tbl(2) pos(tbl(3))=tbl(3) pos(tbl(4))=tbl(4) pos(tbl(5))=tbl(5)
# 
# SATISFIABLE
# Models      : 1+
# Total Steps : 2
# Time        : 0.020
# Prepare   : 0.010
# Prepro.   : 0.000

