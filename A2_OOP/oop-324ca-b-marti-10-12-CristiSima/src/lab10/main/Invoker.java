package lab10.main;

import lab10.commands.*;

import java.util.LinkedList;

/**
 * The layer between the client and the commands that need to be executed on the receivers (DiagramCanvas and DiagramComponent).
 * <br>
 * It is independent of the subtypes of commands, it just receives a command, runs it and implements a redo/undo mechanism.
 */
public class Invoker {

    private LinkedList<DrawCommand> executed;
    private LinkedList<DrawCommand> undone;
    public Invoker() {
        this.executed = new LinkedList();
        this.undone = new LinkedList();
    }
    /**
     * Clear up all the used resources, start fresh :D
     */
    public void restart() {
        this.undone = new LinkedList();
        this.executed = new LinkedList();
    }

    /**
     * Executes a given command
     * @param command
     */
    public void execute(DrawCommand command) {
        this.executed.addLast(command);
        command.execute();
        this.undone = new LinkedList();
    }

    /**
     * Undo the latest command
     */
    public void undo() {
        if (this.executed.size() > 0) {
            DrawCommand toUndone = this.executed.removeLast();
            toUndone.undo();
            this.undone.addLast(toUndone);
        }
    }

    /**
     * Redo command previously undone. Cannot perform a redo after an execute, only after at least one undo.
     */
    public void redo() {
        if (this.undone.size() > 0) {
            DrawCommand toUndone = this.undone.removeLast();
            toUndone.execute();
            this.executed.addLast(toUndone);
        }
    }
}
