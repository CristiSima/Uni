import sys
from antlr4 import *
from parser.CoolParser import CoolParser
from parser.CoolParserVisitor import CoolParserVisitor

class VisitorInterp(CoolParserVisitor):
    '''
    Print directly from Parser
    '''

    indent=0
    spacer="  "
    def print(self, arg, *, indent=None):
        print(self.spacer*self.indent+str(arg))

        if indent == "++":
            self.indent+=1
        elif indent == "--":
            self.indent-=1

    def visitProgram(self, ctx:CoolParser.ProgramContext):
        self.print("program", indent="++")
        for class_ in ctx.class_():
            self.visit(class_)
        self.indent-=1

    def visitClass(self, ctx:CoolParser.ClassContext):
        self.print("class", indent="++")
        self.print(ctx.class_name.text)
        if ctx.base_class:
            self.print(ctx.base_class.text)

        for feature in ctx.features:
            self.visit(feature)
                
        self.indent-=1

    def visitAttribute_definition(self, ctx:CoolParser.Attribute_definitionContext):
        self.print("attribute", indent="++")
        self.print(ctx.attribute_name.text)
        self.print(ctx.attribute_type.text)
        if ctx.default:
            self.visit(ctx.default)
        self.indent-=1
    
    def visitFunction_definition(self, ctx:CoolParser.Function_definitionContext):
        self.print("method", indent="++")
        self.print(ctx.function_name.text)
        for formal in ctx.args:
            self.visit(formal)
        self.print(ctx.return_type.text)
        self.visit(ctx.body)
        self.indent-=1

    def visitFormal(self, ctx:CoolParser.FormalContext):
        self.print("formal", indent="++")
        self.print(ctx.var_name.text)
        self.print(ctx.type_name.text)
        self.indent-=1

    # expr
    def visitAssignment(self, ctx:CoolParser.AssignmentContext):
        self.print(ctx.ASSIGN(), indent="++")
        self.print(ctx.target.text)
        self.visit(ctx.value)
        self.indent-=1

    def visitExplicit_function_call(self, ctx:CoolParser.Explicit_function_callContext):
        self.print(".", indent="++")
        self.visit(ctx.target)
        if ctx.parent_class:
            self.print(ctx.parent_class.text)
        self.print(ctx.method.text)
        for arg in ctx.args:
            self.visit(arg)
        self.indent-=1

    def visitImplicit_function_call(self, ctx:CoolParser.Explicit_function_callContext):
        self.print("implicit dispatch", indent="++")
        self.print(ctx.method.text)
        for arg in ctx.args:
            self.visit(arg)
        self.indent-=1

    def visitIf(self, ctx:CoolParser.IfContext):
        self.print("if", indent="++")
        self.visit(ctx.cond)
        self.visit(ctx.if_true)
        self.visit(ctx.if_false)
        self.indent-=1
        
    def visitWhile(self, ctx:CoolParser.WhileContext):
        self.print("while", indent="++")
        self.visit(ctx.cond)
        self.visit(ctx.action)
        self.indent-=1

    def visitBlock(self, ctx:CoolParser.BlockContext):
        self.print("block", indent="++")
        for expr in ctx.expresions:
            self.visit(expr)
        self.indent-=1

    def visitLet_entry(self, ctx:CoolParser.Let_entryContext):
        self.print("local", indent="++")
        self.print(ctx.var_name.text)
        self.print(ctx.var_type.text)
        if ctx.default_value:
            self.visit(ctx.default_value)
        self.indent-=1

    def visitLet(self, ctx:CoolParser.LetContext):
        self.print("let", indent="++")

        for entry in ctx.entries:
            self.visit(entry)

        self.visit(ctx.expresion)        
        self.indent-=1

    def visitCase(self, ctx:CoolParser.CaseContext):
        self.print("case", indent="++")
        self.visit(ctx.target)
        for var_name, check_type, action in zip(ctx.var_names, ctx.check_types, ctx.actions):
            self.print("case branch", indent="++")
            self.print(var_name.text)
            self.print(check_type.text)
            self.visit(action)
            self.indent-=1
        self.indent-=1

    def visitNew(self, ctx):
        self.print(ctx.NEW(), indent="++")
        self.print(ctx.class_name.text)
        self.indent-=1
    def visitIsvoid(self, ctx):
        self.print(ctx.ISVOID(), indent="++")
        self.visit(ctx.target)
        self.indent-=1
    def visitNegate(self, ctx):
        self.print(ctx.NEGATE(), indent="++")
        self.visit(ctx.target)
        self.indent-=1

    def visitArithm1(self, ctx:CoolParser.Arithm1Context):
        self.print(ctx.opperator.text, indent="++")
        self.visit(ctx.expr1)
        self.visit(ctx.expr2)
        self.indent-=1

    def visitArithm2(self, ctx:CoolParser.Arithm2Context):
        self.print(ctx.opperator.text, indent="++")
        self.visit(ctx.expr1)
        self.visit(ctx.expr2)
        self.indent-=1

    def visitCompare(self, ctx:CoolParser.CompareContext):
        self.print(ctx.comparator.text, indent="++")
        self.visit(ctx.expr1)
        self.visit(ctx.expr2)
        self.indent-=1

    def visitNot(self, ctx):
        self.print(ctx.NOT(), indent="++")
        self.visit(ctx.target)
        self.indent-=1

    def visitParen(self, ctx):
        self.visit(ctx.target)

    def visitId(self, ctx:CoolParser.IdContext):
        self.print(ctx.ID())
    def visitInt(self, ctx):
        self.print(ctx.INT())
    def visitString(self, ctx):
        self.print((ctx.STRING()))
    def visitBool(self, ctx):
        self.print(ctx.BOOL())
    def visitType(self, ctx):
        self.print(ctx.TYPE())