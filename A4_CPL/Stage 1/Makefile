# Set the path to the ANTLR jar here:
ANTLR=/usr/local/lib/antlr4.jar
# This line might get overwritten by the vmchecker.

#
# Main rules:
#
# * build - Cmakompiles everything
# * test - Runs the automated tester
# * clean - Deletes binary files and test-output files 
# * run ARGS=<file> - call compiler
# * run_t t=<test_number:02> - run with test input
# * test t=<test_number:02> [a=<arguments>] - run test input
#
.PHONY: build run run-lexer test zip clean 

build: cool/lexer/CoolLexer.py cool/parser/CooLParser.py

cool/lexer/CoolLexer.py: cool/lexer/CoolLexer.g4 
	antlr4 -v 4.13.1 -Dlanguage=Python3 cool/lexer/CoolLexer.g4 -o cool/lexer
	cp cool/lexer/CoolLexer.tokens cool/parser/

cool/parser/CoolParser.py: cool/parser/CoolParser.g4 cool/lexer/CoolLexer.py
	antlr4 -v 4.13.1 -Dlanguage=Python3 -listener -visitor -o cool/parser -lib cool/lexer cool/parser/CoolParser.g4 

clean:
	rm -rf cool/.antlr/ cool/parser/.antlr/ cool/lexer/.antlr/
	rm -rf cool/__pycache__  cool/parser/__pycache__  cool/lexer/__pycache__
	rm cool/*/*.interp   cool/*/*.py   cool/*/*.tokens
	rm -f ./tests/tema1/*.out out

run: build
	python -X utf8 cool/Compiler.py $(ARGS)

run_t: build
	python -X utf8 cool/Compiler.py ./tests/tema1/$(t)*.cl

test: build
	python -X utf8 cool/Compiler.py ./tests/tema1/$(t)*.cl | diff $(a) tests/tema1/$(t)*.ref -

zip: clean
	rm -f archive.zip
	zip -r archive.zip . --exclude tests/\*