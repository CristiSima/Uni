package lab10.main;

import lab10.commands.*;
import lab10.diagram.DiagramCanvas;

/**
 * Receives commands in clear text from the user and transforms them in DrawCommand objects. It uses the Invoker to
 * execute the given commands.
 */
public class Client {

    private Invoker invoker;
    private DiagramCanvas diagramCanvas;

    Client() {
        invoker = new Invoker();
        diagramCanvas = new DiagramCanvas();
    }

    public void showDiagram() {
        this.diagramCanvas.show();
    }

    public void newDiagram() {
        this.invoker = new Invoker();
        this.diagramCanvas = new DiagramCanvas();
    }

    public void executeAction(String commandName, String ... args) {
        // TODO - uncomment:
        DrawCommand command;
        try {
            CommandType commandType = CommandType.fromString(commandName);
            command = getCommand(commandType, args);
            if (command == null) {
                throw new IllegalArgumentException();
            }

        } catch (IllegalArgumentException ex) {
            System.out.println("Invalid command: " + commandName);
            System.out.println("Available commands:");
            for (CommandType type : CommandType.values()) {
                System.out.println("\t- " + type.text);
            }
            return;
        }

        System.out.println(command);
        this.invoker.execute(command);
    }

    private DrawCommand getCommand(CommandType type, String ... args) throws IllegalArgumentException {
        // TODO factory method to create DrawCommand subclasses.
        // If there is an exception when parsing the string arguments (NumberFormatException) catch it and
        // throw an IllegalArgumentException
        try {
            switch (type) {
            case DRAW_RECTANGLE:
                return new DrawRectangle(this.diagramCanvas);
            case CHANGE_COLOR:
                return new ChangeColor(this.diagramCanvas,
                        Integer.parseInt(args[0]),
                        args[1]);
            case CHANGE_TEXT:
                return new ChangeText(this.diagramCanvas,
                        Integer.parseInt(args[0]),
                        args[1]);
            case RESIZE:
                return new Resize(this.diagramCanvas,
                        Integer.parseInt(args[0]),
                        Integer.parseInt(args[1]),
                        Integer.parseInt(args[2]));
            case CONNECT:
                return new ConnectComponents(this.diagramCanvas,
                        Integer.parseInt(args[0]),
                        Integer.parseInt(args[1]));
            default:
                return null;
            }
        } catch (NumberFormatException ex) {
            throw new IllegalArgumentException();
        }
    }

    public void undo() {
        invoker.undo();
    }

    public void redo() {
        invoker.redo();
    }
}
