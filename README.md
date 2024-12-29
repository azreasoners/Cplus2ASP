# Cplus2ASP V3.1
Translating and solving CCalc action descriptions using answer set solvers

## Introduction
Cplus2ASP is a system designed to perform a modular translation of action descriptions written for CCalc into answer set programs. The current work comprises a suite of tools that translate CCalc action descriptions, solve the translated ASP programs, and produce answer sets similar to the models output by CCalc. If you're unfamiliar with CCalc, it's an implementation of the action language C+ (an extension of causal logic) that is very useful for describing and reasoning about time- and state-based domains using relatively intuitive syntax. For more information, check out the CCalc website using the link in the side bar. Our goal is to combine the user-friendliness of the CCalc language with the speed of modern answer set solvers to capture the best of both worlds.

## Installation on Linux
0. [Optional] Create a virtual environment using conda
```
conda create --name cplus2asp
conda activate cplus2asp
```
1. Clone the current repository, and cd to the folder of Cplus2ASP
```
git clone https://github.com/AdamIshay/Cplus2ASP.git
cd Cplus2ASP
```
2. Install the pre-requiresite packages
```
conda install -c potassco clingo
gcc externals/f2lp/f2lp.c -o f2lp
sudo cp f2lp /usr/local/bin/
sudo apt-get install autoconf automake libtool make
sudo apt-get install libboost-all-dev flex bison re2c
```
3. Grab the current version of boost
```
dpkg -s libboost-dev | grep 'Version'
```
4. Set the env variable "boost_cv_lib_version" to the version of boost you just got in step 3. Say the output of step 3 is "Version: 1.65.1.0ubuntu1".
```
export boost_cv_lib_version=1_65_1
```
5. Install with make file and rename the binary file
```
./bootstrap.sh
make
sudo make install
sudo mv /usr/local/bin/cplus2asp4 /usr/local/bin/cplus2asp
```

## Example Run on Monkey&Banana Example (BC+)
You can test different queries by executing
```
cplus2asp examples/bc+/monkey.bc+ --language=bc+
```
then type a query such as
```
query=2
```

## List of Items in the Cplus2ASP System
### Cplus2ASP
The main orchestrator program of the tool chain acts as a unified interface that accepts all input and options, automatically handling the process of calling the programs below in the right order with the correct configurations. While it is possible to manually call each part of the tool chain separately, we highly recommend letting this system handle the details of organizing and executing the software tool chain.

Cplus2ASP is highly configurable, allowing the tools in the toolchain to be turned off or be substituted for other tools, although compatibility among customly selected toolchain components is not recommened. In addition, Cplus2ASP provides a number of pre-configured running modes, which are detailed below.

| Mode        | Description           |
| ------------- |:-------------:|
| Incremental      | The default running mode. Utilizes a translation to incremental ASP in order to allow an incremental ASP solver to effeciently iterate over all possible steps. |
| Reactive      | This is under development.      |
| Static (auto)	 | Uses the static translation used by previous version of Cplus2ASP. Automatically iterates and resolves for all requested steps until a valid plan is found.      |
| Static (manual)	 | Similar to Static (auto) except that it allows the user to specify the steps to attempt to solve for.      |

### Cplus2ASP.bin
This is the automated translator module for Cplus2ASP. It accepts normal CCalc files as input and translates them into F2LP input. Currently this part of the software tool chain is a prototype, and as such lacks support for some of the more advanced features of CCalc. See the release notes for known limitations of the program.

### F2LP
To allow greater flexibility for the translated programs, we use F2LP as the target language of translation. F2LP supports the use of first-order formulas in its rules, transforming them into equivalent ASP code. A guide to F2LP's syntax and usage can be found on the [F2LP GitHub page](https://github.com/azreasoners/F2LP).

Cplus2ASP v3.1 requires F2LP version 1.4 or higher in order to use the default translation method. If a previous version of F2LP is available the program can still run in static compatibility mode (accessed with the --mode=static-auto argument).

### Clingo3to4
The target language for F2LP is compatible with clingo3. Since the input syntax for clingo3 and clingo4 is not the same we have used this module to translate clingo3 code to clingo4 compatible code.

### Clingo
Cplus2ASP v3.1 has replaced Clingo/iClingo/oClingo system with Clingo 4 as the default ASP grounder/solver suite..

Clingo is an ASP grounder & solver suites produced as part of the Potsdam Answer Set Solving Collection (Potassco) intended to provide higly-optimized static, incremental, and online ASP reasoning and are used by default in the Cplus2ASP respective operating modes.

### as2transition
as2transition is a tool designed to take output of a translated CCalc action description from Clingo (& some other popular answer set solvers) and transform it into a human-readable format similar to the output of CCalc. Using this tool on translated answer sets is almost a must for any non-trivial action description, as anything larger than a simple example will produce answer sets that, while readable, will not be easy to quickly parse and analyze.

## Sample Domains
We've collected "benchmark" problems from CCalc for the purposes of testing and demonstration, and we've made them available as a helpful reference. The Examples link in the sidebar will take you to the page where we've archived all of the examples, including sample domains from the seminal CCalc paper "Nonmonotonic Causal Theories", medium-sized domains called the Zoo World and Traffic World, and a couple of other custom examples we've created or adapted into the language of CCalc. These sample files can be used directly, as they have been set up to be compatible with the translator (Cplus2ASP.bin) as well as the rest of the tool chain.
