# cplus2asp test script file.

######################################################################################################################
# Additive MCP Tests
# --------------------------------------------------------------------------------------------------------------------

# ---------------------------------------------- Missionaries and Cannibals --------------------------------------------
echo "% ----------------------------------------------- Missionairies and Cannibals ---------------------------------------------" 

# --------------------------- Basic ---------------------------
echo "% --------------------------- Basic ---------------------------" 
echo "$ cplus2asp mcp/basic-test query=0 --none-alias" 
cplus2asp mcp/basic-test query=0 --none-alias

# % Warning: Option '--none-alias' has been deprecated in favor of specifying an explicit none alias value.
# % Pragma: One or more additive constants have been detected. Including the additive standard file.
# % Pragma: Found a maxAdditive hint. Setting maxAdditive to '3'.
# % Pragma: Running Query 'Query '0''.
# % Pragma: Minimum Step = '10', Maximum Step = '11'.
# % Pragma: Searching for 1 solutions.
# Solution: 1
#          capacity(boat)=2
# 
#         0:  loc(boat)=bank1 num(ca,bank1)=3 num(ca,bank2)=0 num(mi,bank1)=3 num(mi,bank2)=0
# 
#         ACTIONS:  cross(boat) howmany(boat,ca)=2 howmany(boat,mi)=0 to(boat)=bank2
# 
#         1:  loc(boat)=bank2 num(ca,bank1)=1 num(ca,bank2)=2 num(mi,bank1)=3 num(mi,bank2)=0
# 
#         ACTIONS:  cross(boat) howmany(boat,ca)=1 howmany(boat,mi)=0 to(boat)=bank1
# 
#         2:  loc(boat)=bank1 num(ca,bank1)=2 num(ca,bank2)=1 num(mi,bank1)=3 num(mi,bank2)=0
# 
#         ACTIONS:  cross(boat) howmany(boat,ca)=2 howmany(boat,mi)=0 to(boat)=bank2
# 
#         3:  loc(boat)=bank2 num(ca,bank1)=0 num(ca,bank2)=3 num(mi,bank1)=3 num(mi,bank2)=0
# 
#         ACTIONS:  cross(boat) howmany(boat,ca)=1 howmany(boat,mi)=0 to(boat)=bank1
# 
#         4:  loc(boat)=bank1 num(ca,bank1)=1 num(ca,bank2)=2 num(mi,bank1)=3 num(mi,bank2)=0
# 
#         ACTIONS:  cross(boat) howmany(boat,ca)=0 howmany(boat,mi)=2 to(boat)=bank2
# 
#         5:  loc(boat)=bank2 num(ca,bank1)=1 num(ca,bank2)=2 num(mi,bank1)=1 num(mi,bank2)=2
# 
#         ACTIONS:  cross(boat) howmany(boat,ca)=1 howmany(boat,mi)=1 to(boat)=bank1
# 
#         6:  loc(boat)=bank1 num(ca,bank1)=2 num(ca,bank2)=1 num(mi,bank1)=2 num(mi,bank2)=1
# 
#         ACTIONS:  cross(boat) howmany(boat,ca)=0 howmany(boat,mi)=2 to(boat)=bank2
# 
#         7:  loc(boat)=bank2 num(ca,bank1)=2 num(ca,bank2)=1 num(mi,bank1)=0 num(mi,bank2)=3
# 
#         ACTIONS:  cross(boat) howmany(boat,ca)=1 howmany(boat,mi)=0 to(boat)=bank1
# 
#         8:  loc(boat)=bank1 num(ca,bank1)=3 num(ca,bank2)=0 num(mi,bank1)=0 num(mi,bank2)=3
# 
#         ACTIONS:  cross(boat) howmany(boat,ca)=2 howmany(boat,mi)=0 to(boat)=bank2
# 
#         9:  loc(boat)=bank2 num(ca,bank1)=1 num(ca,bank2)=2 num(mi,bank1)=0 num(mi,bank2)=3
# 
#         ACTIONS:  cross(boat) howmany(boat,ca)=1 howmany(boat,mi)=0 to(boat)=bank1
#
#         10:  loc(boat)=bank1 num(ca,bank1)=2 num(ca,bank2)=1 num(mi,bank1)=0 num(mi,bank2)=3
#
#         ACTIONS:  cross(boat) howmany(boat,ca)=2 howmany(boat,mi)=0 to(boat)=bank2
# 
#         11:  loc(boat)=bank2 num(ca,bank1)=0 num(ca,bank2)=3 num(mi,bank1)=0 num(mi,bank2)=3
# 
# SATISFIABLE
# Models      : 1+
# Total Steps : 11
# Time        : 0.650
# Prepare   : 0.440
# Prepro.   : 0.040


# --------------------------- Mccarthy Elab. 10 ---------------------------
echo "% --------------------------- Mccarthy Elab. 10 ---------------------------" 

echo "$ cplus2asp mcp/jmc10-test query=0 --none-alias" 
cplus2asp mcp/jmc10-test query=0 --none-alias

# % Warning: Option '--none-alias' has been deprecated in favor of specifying an explicit none alias value.
# % Pragma: One or more additive constants have been detected. Including the additive standard file.
# % Pragma: Found a maxAdditive hint. Setting maxAdditive to '3'.
# % Pragma: Running Query 'Query '0''.
# % Pragma: Minimum Step = '0', Maximum Step = '11'.
# % Pragma: Searching for 1 solutions.
# Solution: 1
#          capacity(boat)=2
# 
#         0:  loc(boat)=bank1 num(ca,bank1)=3 num(ca,bank2)=0 num(jc,bank1)=1 num(jc,bank2)=0 num(mi,bank1)=3 num(mi,bank2)=0
# 
#         ACTIONS:  cross(boat) departing(ca,bank1)=2 departing(ca,bank2)=0 departing(jc,bank1)=0 departing(jc,bank2)=0 departing(mi,bank1)=0 departing(mi,bank2)=0 howmany(boat,ca)=2 howmany(boat,jc)=0 howmany(boat,mi)=0 to(boat)=bank2
# 
#         1:  loc(boat)=bank2 num(ca,bank1)=1 num(ca,bank2)=2 num(jc,bank1)=1 num(jc,bank2)=0 num(mi,bank1)=3 num(mi,bank2)=0
# 
#         ACTIONS:  cross(boat) departing(ca,bank1)=0 departing(ca,bank2)=1 departing(jc,bank1)=1 departing(jc,bank2)=0 departing(mi,bank1)=1 departing(mi,bank2)=0 howmany(boat,ca)=1 howmany(boat,jc)=0 howmany(boat,mi)=0 to(boat)=bank1 walk walk_to=bank2
# 
#         2:  loc(boat)=bank1 num(ca,bank1)=2 num(ca,bank2)=1 num(jc,bank1)=0 num(jc,bank2)=1 num(mi,bank1)=2 num(mi,bank2)=1
# 
#         ACTIONS:  cross(boat) departing(ca,bank1)=2 departing(ca,bank2)=0 departing(jc,bank1)=0 departing(jc,bank2)=1 departing(mi,bank1)=0 departing(mi,bank2)=1 howmany(boat,ca)=2 howmany(boat,jc)=0 howmany(boat,mi)=0 to(boat)=bank2 walk walk_to=bank1
# 
#         3:  loc(boat)=bank2 num(ca,bank1)=0 num(ca,bank2)=3 num(jc,bank1)=1 num(jc,bank2)=0 num(mi,bank1)=3 num(mi,bank2)=0
# 
#         ACTIONS:  cross(boat) departing(ca,bank1)=0 departing(ca,bank2)=1 departing(jc,bank1)=0 departing(jc,bank2)=0 departing(mi,bank1)=0 departing(mi,bank2)=0 howmany(boat,ca)=1 howmany(boat,jc)=0 howmany(boat,mi)=0 to(boat)=bank1
# 
#         4:  loc(boat)=bank1 num(ca,bank1)=1 num(ca,bank2)=2 num(jc,bank1)=1 num(jc,bank2)=0 num(mi,bank1)=3 num(mi,bank2)=0
# 
#         ACTIONS:  cross(boat) departing(ca,bank1)=0 departing(ca,bank2)=0 departing(jc,bank1)=1 departing(jc,bank2)=0 departing(mi,bank1)=3 departing(mi,bank2)=0 howmany(boat,ca)=0 howmany(boat,jc)=0 howmany(boat,mi)=2 to(boat)=bank2 walk walk_to=bank2
# 
#         5:  loc(boat)=bank2 num(ca,bank1)=1 num(ca,bank2)=2 num(jc,bank1)=0 num(jc,bank2)=1 num(mi,bank1)=0 num(mi,bank2)=3
# 
#         ACTIONS:  cross(boat) departing(ca,bank1)=0 departing(ca,bank2)=0 departing(jc,bank1)=0 departing(jc,bank2)=0 departing(mi,bank1)=0 departing(mi,bank2)=1 howmany(boat,ca)=0 howmany(boat,jc)=0 howmany(boat,mi)=1 to(boat)=bank1
# 
#         6:  loc(boat)=bank1 num(ca,bank1)=1 num(ca,bank2)=2 num(jc,bank1)=0 num(jc,bank2)=1 num(mi,bank1)=1 num(mi,bank2)=2
# 
#         ACTIONS:  cross(boat) departing(ca,bank1)=1 departing(ca,bank2)=0 departing(jc,bank1)=0 departing(jc,bank2)=0 departing(mi,bank1)=1 departing(mi,bank2)=0 howmany(boat,ca)=1 howmany(boat,jc)=0 howmany(boat,mi)=1 to(boat)=bank2
# 
#         7:  loc(boat)=bank2 num(ca,bank1)=0 num(ca,bank2)=3 num(jc,bank1)=0 num(jc,bank2)=1 num(mi,bank1)=0 num(mi,bank2)=3
# 
# SATISFIABLE
# Models      : 1+
# Total Steps : 7
# Time        : 6.220
# Prepare   : 5.580
# Prepro.   : 0.560

# --------------------------- Mccarthy Elab. 11 ---------------------------
echo "% --------------------------- Mccarthy Elab. 11 ---------------------------" 

echo "$ cplus2asp mcp/jmc11-test query=0 --none-alias" 
cplus2asp mcp/jmc11-test query=0 --none-alias

# % Warning: Option '--none-alias' has been deprecated in favor of specifying an explicit none alias value.
# % Pragma: One or more additive constants have been detected. Including the additive standard file.
# % Pragma: Found a maxAdditive hint. Setting maxAdditive to '6'.
# % Pragma: Running Query 'Query '0''.
# % Pragma: Minimum Step = '0', Maximum Step = '11'.
# % Pragma: Searching for 1 solutions.
# Solution: 1
#          capacity(boat)=2
# 
#         0:  loc(boat)=bank1 num(ca,bank1)=3 num(ca,bank2)=0 num(mi,bank1)=3 num(mi,bank2)=0
# 
#         ACTIONS:  convert(bank1) cross(boat) departing(ca,bank1)=2 departing(ca,bank2)=0 departing(mi,bank1)=0 departing(mi,bank2)=0 howmany(boat,ca)=2 howmany(boat,mi)=0 to(boat)=bank2
# 
#         1:  loc(boat)=bank2 num(ca,bank1)=0 num(ca,bank2)=2 num(mi,bank1)=4 num(mi,bank2)=0
# 
#         ACTIONS:  cross(boat) departing(ca,bank1)=0 departing(ca,bank2)=1 departing(mi,bank1)=0 departing(mi,bank2)=0 howmany(boat,ca)=1 howmany(boat,mi)=0 to(boat)=bank1
# 
#         2:  loc(boat)=bank1 num(ca,bank1)=1 num(ca,bank2)=1 num(mi,bank1)=4 num(mi,bank2)=0
# 
#         ACTIONS:  cross(boat) departing(ca,bank1)=0 departing(ca,bank2)=0 departing(mi,bank1)=2 departing(mi,bank2)=0 howmany(boat,ca)=0 howmany(boat,mi)=2 to(boat)=bank2
# 
#         3:  loc(boat)=bank2 num(ca,bank1)=1 num(ca,bank2)=1 num(mi,bank1)=2 num(mi,bank2)=2
# 
#         ACTIONS:  cross(boat) departing(ca,bank1)=0 departing(ca,bank2)=1 departing(mi,bank1)=0 departing(mi,bank2)=0 howmany(boat,ca)=1 howmany(boat,mi)=0 to(boat)=bank1
# 
#         4:  loc(boat)=bank1 num(ca,bank1)=2 num(ca,bank2)=0 num(mi,bank1)=2 num(mi,bank2)=2
# 
#         ACTIONS:  cross(boat) departing(ca,bank1)=2 departing(ca,bank2)=0 departing(mi,bank1)=0 departing(mi,bank2)=0 howmany(boat,ca)=2 howmany(boat,mi)=0 to(boat)=bank2
# 
#         5:  loc(boat)=bank2 num(ca,bank1)=0 num(ca,bank2)=2 num(mi,bank1)=2 num(mi,bank2)=2
# 
#         ACTIONS:  cross(boat) departing(ca,bank1)=0 departing(ca,bank2)=1 departing(mi,bank1)=0 departing(mi,bank2)=0 howmany(boat,ca)=1 howmany(boat,mi)=0 to(boat)=bank1
# 
#         6:  loc(boat)=bank1 num(ca,bank1)=1 num(ca,bank2)=1 num(mi,bank1)=2 num(mi,bank2)=2
# 
#         ACTIONS:  cross(boat) departing(ca,bank1)=0 departing(ca,bank2)=0 departing(mi,bank1)=2 departing(mi,bank2)=0 howmany(boat,ca)=0 howmany(boat,mi)=2 to(boat)=bank2
# 
#         7:  loc(boat)=bank2 num(ca,bank1)=1 num(ca,bank2)=1 num(mi,bank1)=0 num(mi,bank2)=4
# 
#         ACTIONS:  convert(bank2) cross(boat) departing(ca,bank1)=0 departing(ca,bank2)=0 departing(mi,bank1)=0 departing(mi,bank2)=1 howmany(boat,ca)=0 howmany(boat,mi)=1 to(boat)=bank1
# 
#         8:  loc(boat)=bank1 num(ca,bank1)=1 num(ca,bank2)=0 num(mi,bank1)=1 num(mi,bank2)=4
# 
#         ACTIONS:  cross(boat) departing(ca,bank1)=1 departing(ca,bank2)=0 departing(mi,bank1)=1 departing(mi,bank2)=0 howmany(boat,ca)=1 howmany(boat,mi)=1 to(boat)=bank2
# 
#         9:  loc(boat)=bank2 num(ca,bank1)=0 num(ca,bank2)=1 num(mi,bank1)=0 num(mi,bank2)=5
# 
# SATISFIABLE
# Models      : 1+
# Total Steps : 9
# Time        : 10.900
# Prepare   : 9.410
# Prepro.   : 1.060

# --------------------------- Mccarthy Elab. 13 ---------------------------
echo "% --------------------------- Mccarthy Elab. 13 ---------------------------" 

echo "$ cplus2asp mcp/jmc13-test query=0 --none-alias" 
cplus2asp mcp/jmc13-test query=0 --none-alias

# % Warning: Option '--none-alias' has been deprecated in favor of specifying an explicit none alias value.
# % Pragma: One or more additive constants have been detected. Including the additive standard file.
# % Pragma: Found a maxAdditive hint. Setting maxAdditive to '5'.
# % Pragma: Running Query 'Query '0''.
# % Pragma: Minimum Step = '0', Maximum Step = '11'.
# % Pragma: Searching for 1 solutions.
# Solution: 1
#          capacity(boat)=2
# 
#         0:  loc(boat)=bank1 num(ca,bank1)=5 num(ca,bank2)=0 num(mi,bank1)=5 num(mi,bank2)=0
# 
#         ACTIONS:  departing(ca,bank1)=2 departing(ca,bank2)=0 departing(mi,bank1)=0 departing(mi,bank2)=0 useBridge useBridge_from=bank1 useBridge_howmany(ca)=2 useBridge_howmany(mi)=0 useBridge_to=bank2
# 
#         1:  loc(boat)=bank1 num(ca,bank1)=3 num(ca,bank2)=2 num(mi,bank1)=5 num(mi,bank2)=0
# 
#         ACTIONS:  departing(ca,bank1)=2 departing(ca,bank2)=0 departing(mi,bank1)=0 departing(mi,bank2)=0 useBridge useBridge_from=bank1 useBridge_howmany(ca)=2 useBridge_howmany(mi)=0 useBridge_to=bank2
# 
#         2:  loc(boat)=bank1 num(ca,bank1)=1 num(ca,bank2)=4 num(mi,bank1)=5 num(mi,bank2)=0
# 
#         ACTIONS:  cross(boat) departing(ca,bank1)=0 departing(ca,bank2)=0 departing(mi,bank1)=4 departing(mi,bank2)=0 howmany(boat,ca)=0 howmany(boat,mi)=2 to(boat)=bank2 useBridge useBridge_from=bank1 useBridge_howmany(ca)=0 useBridge_howmany(mi)=2 useBridge_to=bank2
# 
#         3:  loc(boat)=bank2 num(ca,bank1)=1 num(ca,bank2)=4 num(mi,bank1)=1 num(mi,bank2)=4
# 
#         ACTIONS:  departing(ca,bank1)=1 departing(ca,bank2)=0 departing(mi,bank1)=1 departing(mi,bank2)=0 useBridge useBridge_from=bank1 useBridge_howmany(ca)=1 useBridge_howmany(mi)=1 useBridge_to=bank2
# 
#         4:  loc(boat)=bank2 num(ca,bank1)=0 num(ca,bank2)=5 num(mi,bank1)=0 num(mi,bank2)=5
# 
# SATISFIABLE
# Models      : 1+
# Total Steps : 4
# Time        : 3.850
# Prepare   : 3.470
# Prepro.   : 0.320

# ------------------------------ Two Boats ------------------------------
echo "% ------------------------------ Two Boats ------------------------------" 

echo "$ cplus2asp mcp/two-boats-test query=0 --none-alias" 
cplus2asp mcp/two-boats-test query=0 --none-alias

# % Warning: Option '--none-alias' has been deprecated in favor of specifying an explicit none alias value.
# % Pragma: One or more additive constants have been detected. Including the additive standard file.
# % Pragma: Found a maxAdditive hint. Setting maxAdditive to '3'.
# % Pragma: Running Query 'Query '0''.
# % Pragma: Minimum Step = '0', Maximum Step = '11'.
# % Pragma: Searching for 1 solutions.
# Solution: 1
#          capacity(boat)=2 capacity(boat1)=1
# 
#         0:  loc(boat)=bank1 loc(boat1)=bank1 num(ca,bank1)=3 num(ca,bank2)=0 num(mi,bank1)=3 num(mi,bank2)=0
# 
#         ACTIONS:  cross(boat) departing(ca,bank1)=2 departing(ca,bank2)=0 departing(mi,bank1)=0 departing(mi,bank2)=0 howmany(boat,ca)=2 howmany(boat,mi)=0 to(boat)=bank2
# 
#         1:  loc(boat)=bank2 loc(boat1)=bank1 num(ca,bank1)=1 num(ca,bank2)=2 num(mi,bank1)=3 num(mi,bank2)=0
# 
#         ACTIONS:  cross(boat) departing(ca,bank1)=0 departing(ca,bank2)=1 departing(mi,bank1)=0 departing(mi,bank2)=0 howmany(boat,ca)=1 howmany(boat,mi)=0 to(boat)=bank1
# 
#         2:  loc(boat)=bank1 loc(boat1)=bank1 num(ca,bank1)=2 num(ca,bank2)=1 num(mi,bank1)=3 num(mi,bank2)=0
# 
#         ACTIONS:  cross(boat) cross(boat1) departing(ca,bank1)=0 departing(ca,bank2)=0 departing(mi,bank1)=3 departing(mi,bank2)=0 howmany(boat,ca)=0 howmany(boat,mi)=2 howmany(boat1,ca)=0 howmany(boat1,mi)=1 to(boat)=bank2 to(boat1)=bank2
# 
#         3:  loc(boat)=bank2 loc(boat1)=bank2 num(ca,bank1)=2 num(ca,bank2)=1 num(mi,bank1)=0 num(mi,bank2)=3
# 
#         ACTIONS:  cross(boat) cross(boat1) departing(ca,bank1)=0 departing(ca,bank2)=0 departing(mi,bank1)=0 departing(mi,bank2)=2 howmany(boat,ca)=0 howmany(boat,mi)=1 howmany(boat1,ca)=0 howmany(boat1,mi)=1 to(boat)=bank1 to(boat1)=bank1
# 
#         4:  loc(boat)=bank1 loc(boat1)=bank1 num(ca,bank1)=2 num(ca,bank2)=1 num(mi,bank1)=2 num(mi,bank2)=1
# 
#         ACTIONS:  cross(boat) cross(boat1) departing(ca,bank1)=1 departing(ca,bank2)=0 departing(mi,bank1)=2 departing(mi,bank2)=0 howmany(boat,ca)=1 howmany(boat,mi)=1 howmany(boat1,ca)=0 howmany(boat1,mi)=1 to(boat)=bank2 to(boat1)=bank2
# 
#         5:  loc(boat)=bank2 loc(boat1)=bank2 num(ca,bank1)=1 num(ca,bank2)=2 num(mi,bank1)=0 num(mi,bank2)=3
# 
#         ACTIONS:  cross(boat) departing(ca,bank1)=0 departing(ca,bank2)=0 departing(mi,bank1)=0 departing(mi,bank2)=1 howmany(boat,ca)=0 howmany(boat,mi)=1 to(boat)=bank1
# 
#         6:  loc(boat)=bank1 loc(boat1)=bank2 num(ca,bank1)=1 num(ca,bank2)=2 num(mi,bank1)=1 num(mi,bank2)=2
# 
#         ACTIONS:  cross(boat) departing(ca,bank1)=1 departing(ca,bank2)=0 departing(mi,bank1)=1 departing(mi,bank2)=0 howmany(boat,ca)=1 howmany(boat,mi)=1 to(boat)=bank2
# 
#         7:  loc(boat)=bank2 loc(boat1)=bank2 num(ca,bank1)=0 num(ca,bank2)=3 num(mi,bank1)=0 num(mi,bank2)=3
# 
# SATISFIABLE
# Models      : 1+
# Total Steps : 7
# Time        : 2.520
# Prepare   : 1.940
# Prepro.   : 0.230

