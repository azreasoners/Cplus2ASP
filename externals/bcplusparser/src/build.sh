echo "=================================================================================================================================="
echo "=================================================================================================================================="
echo "=================================================================================================================================="
echo "=================================================================================================================================="
echo "=================================================================================================================================="
re2c Configuration.re2c > Configuration.cpp
re2c -c parser/Scanner.re2c > parser/Scanner.cpp
lemon parser/atom_parser.y
g++ Configuration.cpp main.cpp Module.cpp ModuleGraph.cpp ModuleNode.cpp Referenced.cpp AtomParser.cpp Solution.cpp parser/atom_parser.c parser/BaseType.cpp parser/BaseElement.cpp parser/Scanner.cpp -lboost_filesystem -lboost_system -o masp -O0 -g -I.
