ARGS= -cp .;"../libs/*"

build: main

checker: common checker/Checker.class

checker/Checker.class:  checker/Checker.java
	javac $(ARGS) checker/*.java

enums: enums/Category.class enums/Cities.class

enums/Category.class: enums/Category.java
	javac $(ARGS) enums/Category.java

enums/Cities.class: enums/Cities.java
	javac $(ARGS) enums/Cities.java

common: common/Constants.class

common/Constants.class: common/Constants.java
	javac $(ARGS) common/Constants.java

main: elements checker ioutils common simulation main/Main.class main/Test.class
main/Main.class: main/Main.java
	javac $(ARGS) main/Main.java
main/Test.class: main/Test.java
	javac $(ARGS) main/Test.java

ioutils: elements ioutils/InputData.class ioutils/Writer.class

ioutils/ioUtils.class: ioutils/ioUtils.java
	javac $(ARGS) ioutils/ioUtils.java

ioutils/InputData.class: ioutils/InputData.java
	javac $(ARGS) ioutils/InputData.java

ioutils/Writer.class: ioutils/Writer.java
	javac $(ARGS) ioutils/Writer.java

elements: elements/Gift.class elements/Child.class elements/AnualChange.class

elements/Gift.class: elements/Gift.java
	javac $(ARGS) elements/Gift.java

elements/Child.class: elements/Child.java
	javac $(ARGS) elements/Child.java

elements/AnualChange.class: elements/AnualChange.java
	javac $(ARGS) elements/AnualChange.java

simulation: enums elements simulation/Simulation.class
simulation/Simulation.class: simulation/Simulation.java
	javac $(ARGS) simulation/Simulation.java

.PHONY: check test
check: main
	java $(ARGS) main.Main
test: main
	java $(ARGS) main.Test
