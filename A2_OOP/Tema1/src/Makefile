ARGS= -cp .;"../libs/*"

build:actor common entertainment fileio checker utils main actions systems

actor:
	javac $(ARGS) actor/*.java

common:
	javac $(ARGS) common/*.java

entertainment:
	javac $(ARGS) entertainment/*.java

fileio: utils/Utils common entertainment
	javac $(ARGS) fileio/*.java

checker:
	javac $(ARGS) checker/*.java

utils/Utils:
	javac $(ARGS) utils/Utils.java

utils: systems
	javac $(ARGS) utils/*.java

main: checker fileio systems actions
	javac $(ARGS) main/*.java

actions: fileio systems utils
	javac $(ARGS) actions/*.java

systems: fileio entertainment
	javac $(ARGS) systems/*.java

# clean:
# 	rm */*.class

.PHONY: test check
test:
	java $(ARGS) main.Test
check:
	java $(ARGS) main.Main

.PHONY: build actor common entertainment fileio main checker actions systems
