from ASTNode import ASTNode

__all__ = [
    "ASTVisitor",
    "AST_visits"
]

class ASTVisitor:
    __new_vistor_map__ = {}
    '''
    Uses the instanciation of classes and decorators to create a visitor like experience

    It is a class visitor to be more precice
    '''
    def __init_subclass__(cls):
        cls.visit_map = __class__.__new_vistor_map__
        # reset for the next visitor
        __class__.__new_vistor_map__ = {}

    @classmethod
    def visit(cls, node: ASTNode):
        cls.visit_map[node.__class__](node)

    def ast_visits(ast_node_type):
        def temp(method):
            # add to the visitor map for the next visitor
            __class__.__new_vistor_map__[ast_node_type] = method
            return method
        return temp

# decorator
AST_visits = ASTVisitor.ast_visits

if __name__ == "__main__":
    class ASTPrintVisitor(ASTVisitor):
        @AST_visits(int)
        @AST_visits(str)
        def print(arg):
            print(arg)

    ASTPrintVisitor.visit(2)
    ASTPrintVisitor.visit("asdasd")

    class ASTInspectVisitor(ASTVisitor):
        @AST_visits(int)
        def print(arg):
            print("print_int: ", arg)
        @AST_visits(str)
        def print(arg):
            print("print_str: ", arg)

    ASTInspectVisitor.visit(2)
    ASTInspectVisitor.visit("asdasd")