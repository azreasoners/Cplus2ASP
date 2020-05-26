# cplus2asp test script file.

######################################################################################################################
# LUA Examples
# --------------------------------------------------------------------------------------------------------------------

# ----------------------------------------------------- LUA Examples ---------------------------------------------------
echo "% ------------------------------------------------------ LUA Examples ----------------------------------------------------" 

# --------------------------- Simple ---------------------------
echo "% --------------------------- Simple ---------------------------" 
echo "$ cplus2asp lua/simple-lua.cp query=states" 
cplus2asp lua/simple-lua.cp query=states


# Pragma: Running Query 'System states'.
# % Pragma: Minimum Step = '0', Maximum Step = '0'.
# % Pragma: Searching for 1 solutions.
# Solution: 1
#          computed(7)
# 
#         0:  p(7)
# 
# SATISFIABLE
# Models      : 1
# Time        : 0.000
# Prepare   : 0.000
# Prepro.   : 0.000

# --------------------------- Simple2 ---------------------------
echo "% --------------------------- Simple2 ---------------------------" 
echo "$ cplus2asp lua/simple-lua-2.cp query=states 0" 
cplus2asp lua/simple-lua-2.cp query=states 0

# % Pragma: Running Query 'System states'.
# % Pragma: Minimum Step = '0', Maximum Step = '0'.
# % Pragma: Searching for all solutions.
# Solution: 1
#         0:  even=1 n=2
# 
# Solution: 2
#         0:  even=1 n=4
# 
# Solution: 3
#         0:  even=1 n=6
# 
# Solution: 4
#         0:  even=1 n=8
# 
# Solution: 5
#         0:  even=1 n=10
# 
# Solution: 6
#         0:  even=0 n=1
# 
# Solution: 7
#         0:  even=0 n=3
# 
# Solution: 8
#         0:  even=0 n=5
# 
# Solution: 9
#         0:  even=0 n=7
# 
# Solution: 10
#         0:  even=0 n=9
# 
# SATISFIABLE
# Models      : 10
# Time        : 0.000
# Prepare   : 0.000
# Prepro.   : 0.000
 
 
# --------------------------- Random ---------------------------
echo "% --------------------------- Random ---------------------------" 
echo "$ cplus2asp lua/random.cp query=states" 
cplus2asp lua/random.cp query=states

# % Pragma: Running Query 'System states'.
# % Pragma: Minimum Step = '0', Maximum Step = '0'.
# % Pragma: Searching for 1 solutions.
# Solution: 1
#          r=98
# 
# SATISFIABLE
# Models      : 1
# Time        : 0.000
# Prepare   : 0.000
# Prepro.   : 0.000


# --------------------------- BW Path ---------------------------
echo "% --------------------------- BW Path ---------------------------" 
echo "$ cplus2asp lua/bw-path.bc query=stack -l bc" 
cplus2asp lua/bw-path.bc query=stack -l bc

# % Pragma: Running Query 'Query 'stack''.
# % Pragma: Minimum Step = '0', No Maximum Step.
# % Pragma: Searching for 1 solutions.
# Solution: 1
#          computed(1)
# 
#         0:  height(blk(1))=1 height(blk(2))=1 height(blk(3))=1 height(blk(4))=1 height(tbl(1))=0 height(tbl(2))=0 height(tbl(3))=0 height(tbl(4))=0 height(tbl(5))=0 on(blk(1))=tbl(1) on(blk(2))=tbl(2) on(blk(3))=tbl(3) on(blk(4))=tbl(4) on(tbl(1))=blk(1) on(tbl(2))=blk(1) on(tbl(3))=blk(1) on(tbl(4))=blk(1) on(tbl(5))=blk(1) pos(blk(1))=tbl(1) pos(blk(2))=tbl(2) pos(blk(3))=tbl(3) pos(blk(4))=tbl(4) pos(tbl(1))=tbl(1) pos(tbl(2))=tbl(2) pos(tbl(3))=tbl(3) pos(tbl(4))=tbl(4) pos(tbl(5))=tbl(5)
# 
#         ACTIONS:  begin_pos(left,tbl(3),1) begin_pos(right,tbl(1),1) dest_pos(left,tbl(5),1) dest_pos(right,tbl(4),1) move(left,blk(3),tbl(5)) move(right,blk(1),blk(4))
# 
#         1:  height(blk(1))=2 height(blk(2))=1 height(blk(3))=1 height(blk(4))=1 height(tbl(1))=0 height(tbl(2))=0 height(tbl(3))=0 height(tbl(4))=0 height(tbl(5))=0 on(blk(1))=blk(4) on(blk(2))=tbl(2) on(blk(3))=tbl(5) on(blk(4))=tbl(4) on(tbl(1))=blk(1) on(tbl(2))=blk(1) on(tbl(3))=blk(1) on(tbl(4))=blk(1) on(tbl(5))=blk(1) pos(blk(1))=tbl(4) pos(blk(2))=tbl(2) pos(blk(3))=tbl(5) pos(blk(4))=tbl(4) pos(tbl(1))=tbl(1) pos(tbl(2))=tbl(2) pos(tbl(3))=tbl(3) pos(tbl(4))=tbl(4) pos(tbl(5))=tbl(5)
# 
#         ACTIONS:  begin_pos(left,tbl(4),1) begin_pos(right,tbl(5),1) dest_pos(left,tbl(2),1) dest_pos(right,tbl(4),1) move(left,blk(4),blk(2)) move(right,blk(3),blk(1))
# 
#         2:  height(blk(1))=3 height(blk(2))=1 height(blk(3))=4 height(blk(4))=2 height(tbl(1))=0 height(tbl(2))=0 height(tbl(3))=0 height(tbl(4))=0 height(tbl(5))=0 on(blk(1))=blk(4) on(blk(2))=tbl(2) on(blk(3))=blk(1) on(blk(4))=blk(2) on(tbl(1))=blk(1) on(tbl(2))=blk(1) on(tbl(3))=blk(1) on(tbl(4))=blk(1) on(tbl(5))=blk(1) pos(blk(1))=tbl(2) pos(blk(2))=tbl(2) pos(blk(3))=tbl(2) pos(blk(4))=tbl(2) pos(tbl(1))=tbl(1) pos(tbl(2))=tbl(2) pos(tbl(3))=tbl(3) pos(tbl(4))=tbl(4) pos(tbl(5))=tbl(5)
# 
# SATISFIABLE
# Models      : 1+
# Total Steps : 2
# Time        : 5.940
# Prepare   : 5.930
# Prepro.   : 0.010

