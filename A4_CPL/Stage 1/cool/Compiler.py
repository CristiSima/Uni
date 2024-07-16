import sys
import os
from antlr4 import *
from antlr4.error.ErrorListener import *
from antlr4.error import Errors
from lexer.CoolLexer import CoolLexer as BaseCoolLexer
from parser.CoolParser import CoolParser
from VisitorInterp import VisitorInterp
from VistorASTBuilder import VistorASTBuilder
from PrintASTVisitor import PrintASTVisitor

class ErrorListener(ErrorListener):
    def __init__(self, filename):
        self.filename=filename
        self.errors = False

    def syntaxError(self, recognizer, offendingSymbol, line, column, msg, e):        
        new_msg = f"\"{self.filename}\", line {line}:{column+1}, "

        if isinstance(e, Errors.LexerNoViableAltException):
            target=e.input.getText(e.startIndex, e.startIndex)
            new_msg+=f"Lexical error: Invalid character: {target}"
        elif offendingSymbol.type == CoolLexer.ERROR:
            new_msg+=f"Lexical error: {offendingSymbol.text}"
        else:
            new_msg+=f"Syntax error: {msg}"
        
        print(new_msg)
        self.errors = True

class CoolLexer(BaseCoolLexer):
    '''
    bc @lexer::members doesn't work
    lexer with methods used in the lexer.g
    + error definition
    + string processing
    '''
    def mark_error(self, msg):
        self.type=self.ERROR
        self.text=msg
        return msg

    def clean_string(self, raw):
        # trim quetos 
        value=str(raw)[1:-1]

        # handle escape sequences
        result=""
        while "\\" in value:
            before, after=value.split("\\", 1)
            c, after = after[0], after[1:]
            if c == "\n":
                c="\n"
            if c == "n":
                c="\n"
            elif c == "t":
                c="\t"
            result += before + c
            value = after

        result+=value

        if len(result) >= 1024:
            return self.mark_error("String constant too long")
        return result

def main(argv):
    if len(argv)<2:
        print("Give me the file")
        exit(1)
    input_stream = FileStream(argv[1], encoding="utf-8")
    errorListener = ErrorListener(os.path.basename(argv[1]))
    lexer = CoolLexer(input_stream)
    lexer.removeErrorListeners()
    lexer.addErrorListener(errorListener)
    stream = CommonTokenStream(lexer)
    parser = CoolParser(stream)
    parser.removeErrorListeners()
    parser.addErrorListener(errorListener)
    tree = parser.program()
    if errorListener.errors:
        print("Compilation halted")
        return


    print_from_ast = True

    if print_from_ast:
        ast_tree = VistorASTBuilder().visit(tree)
        PrintASTVisitor.visit(ast_tree)
    else:
        vinterp = VisitorInterp()
        vinterp.visit(tree)

if __name__ == '__main__':
    main(sys.argv)


# antlr4 -v 4.13.1 -Dlanguage=Python3 -listener -visitor  Expr.g4
# python.exe .\Main.py input.txt