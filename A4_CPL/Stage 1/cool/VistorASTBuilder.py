from ASTNode import *
from antlr4 import *
from parser.CoolParser import CoolParser
from parser.CoolParserVisitor import CoolParserVisitor

def getId(token: Token) -> Id:
    return Id(str(token.text))
def getInt(token: Token) -> INT:
    return INT(int(str(token.text)))
def getString(token: Token) -> String:
    return String(str(token.text))
def getBool(token: Token) -> Bool:
    return Bool(str(token.text)=="true")
def getType(token: Token) -> Type:
    return Type(str(token.text))

def map_list(func, *args): return list(map(func, *args))

class VistorASTBuilder(CoolParserVisitor):
    def visitProgram(self, ctx:CoolParser.ProgramContext):
        return Program([
            self.visit(class_) for class_ in ctx.class_() 
        ])
    
    def visitClass(self, ctx:CoolParser.ClassContext):
        base_class = None
        if ctx.base_class:
            base_class = getType(ctx.base_class)
        return Class(
            getType(ctx.class_name),
            base_class,
            map_list(self.visit, ctx.features)
        )

    def visitAttribute_definition(self, ctx:CoolParser.Attribute_definitionContext):
        return AttributeDefinition(
            getId(ctx.attribute_name),
            getType(ctx.attribute_type),
            self.visit(ctx.default) if ctx.default else None
        )

    def visitFunction_definition(self, ctx:CoolParser.Function_definitionContext):
        return FunctionDefinition(
            getId(ctx.function_name),
            map_list(self.visitFormal, ctx.args),
            getType(ctx.return_type),
            self.visit(ctx.body)
        )

    def visitFormal(self, ctx:CoolParser.FormalContext):
        return (
            getType(ctx.type_name),
            getId(ctx.var_name)
        )

    def visitAssignment(self, ctx:CoolParser.AssignmentContext):
        return Assign(
            getId(ctx.target),
            self.visit(ctx.value)
        )

    def visitExplicit_function_call(self, ctx:CoolParser.Explicit_function_callContext):
        return ExplicitCall(
            self.visit(ctx.target),
            getType(ctx.parent_class) if ctx.parent_class else None,
            getId(ctx.method),
            map_list(self.visit, ctx.args)
        )

    def visitImplicit_function_call(self, ctx:CoolParser.Explicit_function_callContext):
        return ImplicitCall(
            getId(ctx.method),
            map_list(self.visit, ctx.args)
        )

    def visitIf(self, ctx:CoolParser.IfContext):
        return If(
            self.visit(ctx.cond),
            self.visit(ctx.if_true),
            self.visit(ctx.if_false),
        )

    def visitWhile(self, ctx:CoolParser.WhileContext):
        return While(
            self.visit(ctx.cond),
            self.visit(ctx.action)
        )

    def visitBlock(self, ctx:CoolParser.BlockContext):
        return Block(
            map_list(self.visit, ctx.expresions)
        )

    def visitLet_entry(self, ctx:CoolParser.Let_entryContext):
        return (
            getId(ctx.var_name),
            getType(ctx.var_type),
            self.visit(ctx.default_value) if ctx.default_value else None
        )

    def visitLet(self, ctx:CoolParser.LetContext):
        return Let(
            map_list(self.visitLet_entry, ctx.entries),
            self.visit(ctx.expresion)
        )

    def visitCase(self, ctx:CoolParser.CaseContext):
        return Case(
            self.visit(ctx.target),
            [(
                getId(var_name),
                getType(check_type),
                self.visit(action)
            )   for var_name, check_type, action 
                in zip(ctx.var_names, ctx.check_types, ctx.actions) ]
        )

    def visitNew(self, ctx):
        return New(
            getType(ctx.class_name)
        )

    def visitIsvoid(self, ctx):
        return IsVoid(
            self.visit(ctx.target)
        )

    def visitNegate(self, ctx):
        return Negate(
            self.visit(ctx.target)
        )

    def visitArithm1(self, ctx:CoolParser.Arithm1Context):
        return Arithmetic(
            self.visit(ctx.expr1),
            self.visit(ctx.expr2),
            ctx.opperator.text
        )

    def visitArithm2(self, ctx:CoolParser.Arithm2Context):
        return Arithmetic(
            self.visit(ctx.expr1),
            self.visit(ctx.expr2),
            ctx.opperator.text
        )
        
    def visitCompare(self, ctx:CoolParser.CompareContext):
        return Comparison(
            self.visit(ctx.expr1),
            self.visit(ctx.expr2),
            ctx.comparator.text
        )

    def visitNot(self, ctx):
        return Not(
            self.visit(ctx.target)
        )

    def visitParen(self, ctx):
        return self.visit(ctx.target)

    def visitId(self, ctx:CoolParser.IdContext) -> Id:
        return Id(str(ctx.ID()))
    def visitInt(self, ctx) -> INT:
        return INT(int(str(ctx.INT())))
    def visitString(self, ctx) -> String:
        return String(str(ctx.STRING()))
    def visitBool(self, ctx) -> Bool:
        return Bool(str(ctx.BOOL())=="true")
    def visitType(self, ctx) -> Type:
        return Type(str(ctx.TYPE()))