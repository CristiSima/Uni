from antlr4.Lexer import Token
from typing import List, Tuple
from dataclasses import dataclass

__all__ = [
    "ASTNode",
    "Id", "Type", "INT", "Bool", "String",
    "Block", "While", "If", "Case", "Let",
    "Negate", "IsVoid", "Not", "Arithmetic", "Comparison",
    "ImplicitCall", "ExplicitCall",
    "Instanciation", "Assign", "New",
    "FunctionDefinition", "AttributeDefinition", "Class", "Program",
]

@dataclass
class ASTNode:
    pass
class Expresion(ASTNode):
    pass

@dataclass
class Type(Expresion):
    value: str

@dataclass
class Id(Expresion):
    value: str

@dataclass
class INT(Expresion):
    value: int

@dataclass
class String(Expresion):
    value: str

@dataclass
class Bool(Expresion):
    value: bool

@dataclass
class Instanciation(Expresion):
    type_name:  Type

@dataclass
class Case(Expresion):
    target:     Expresion
    cases:      List[Tuple[Id, Type, Expresion]]

@dataclass
class Let(Expresion):
    entries:    List[Tuple[Id, Type, Expresion]]
    body:       Expresion

@dataclass
class Block(Expresion):
    expresions:     List[Expresion]

@dataclass
class While(Expresion):
    condition:      Expresion
    body:           Expresion

@dataclass
class If(Expresion):
    condition:      Expresion
    is_true:        Expresion
    is_false:       Expresion

@dataclass
class ImplicitCall(Expresion):
    name:           Id
    args:           List[Expresion]

@dataclass
class ExplicitCall(Expresion):
    target:         Expresion
    upcall_target:  Type
    name:           Id
    args:           List[Expresion]

@dataclass
class Negate(Expresion):
    target:         Expresion

@dataclass
class IsVoid(Expresion):
    target:         Expresion

@dataclass
class Not(Expresion):
    target:         Expresion

@dataclass
class Assign(Expresion):
    target:         Id
    new_value:      Expresion

@dataclass
class New(Expresion):
    target:         Type

@dataclass
class Arithmetic(Expresion):
    elem1:          Expresion
    elem2:          Expresion
    opperator:      str

@dataclass
class Comparison(Expresion):
    elem1:          Expresion
    elem2:          Expresion
    opperator:      str

@dataclass
class FunctionDefinition(ASTNode):
    name:           Id
    arguments:      List[Tuple[Type, Id]]
    return_type:    Type
    body:           Expresion

@dataclass
class AttributeDefinition(ASTNode):
    name:           Id
    type_name:      Type
    default_value:  Expresion

@dataclass
class Class(ASTNode):
    name:           Type
    base_class:     Type
    features:       List[FunctionDefinition|AttributeDefinition]
    
@dataclass
class Program(ASTNode):
    classes:        List[Class]


