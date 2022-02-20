ARGS= -cp .;"../libs/*"

build: main

checker: common checker/Checker.class

checker/Checker.class:  checker/Checker.java
	javac $(ARGS) checker/*.java

enums: enums/Category.class enums/Cities.class enums/ElvesType.class enums/StrategyEnum.class

enums/Category.class: enums/Category.java
	javac $(ARGS) enums/Category.java

enums/Cities.class: enums/Cities.java
	javac $(ARGS) enums/Cities.java

enums/StrategyEnum.class: enums/StrategyEnum.java
	javac $(ARGS) enums/StrategyEnum.java

enums/ElvesType.class: enums/ElvesType.java
	javac $(ARGS) enums/ElvesType.java

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

elements/Child.class: enums strategies/AverageNiceScore.class elements/Child.java
	javac $(ARGS) elements/Child.java

elements/AnualChange.class: enums/StrategyEnum.class elements/AnualChange.java
	javac $(ARGS) elements/AnualChange.java

strategies: strategies/AverageNiceScore.class strategies/GiftAssignmentOrder.class strategies/OnBudget.class strategies/OnGifts.class

strategies/AverageNiceScore.class: strategies/AverageNiceScore.java strategies/OnBudget.class
	javac $(ARGS) strategies/AverageNiceScore.java

strategies/OnBudget.class: strategies/OnBudget.java
	javac $(ARGS) strategies/OnBudget.java

strategies/OnGifts.class: simulation/SimulationStorage.class strategies/OnGifts.java
	javac $(ARGS) strategies/OnGifts.java

strategies/GiftAssignmentOrder.class: strategies/GiftAssignmentOrder.java
	javac $(ARGS) strategies/GiftAssignmentOrder.java

simulation: simulation/Simulation.class simulation/SimulationStorage.class
simulation/Simulation.class: enums elements strategies simulation/Simulation.java
	javac $(ARGS) simulation/Simulation.java
simulation/SimulationStorage.class: enums elements simulation/SimulationStorage.java
	javac $(ARGS) simulation/SimulationStorage.java

.PHONY: check test
check: main
	java $(ARGS) main.Main
test: main
	java $(ARGS) main.Test
