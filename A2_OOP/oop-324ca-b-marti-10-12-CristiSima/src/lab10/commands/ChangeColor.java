package lab10.commands;

import lab10.diagram.*;

public class ChangeColor implements DrawCommand {
    private DiagramCanvas diagramCanvas;
    private DiagramComponent diagramComponent;

    private String oldColor;
    private String newColor;

    public ChangeColor(DiagramCanvas diagramCanvas, int diagramComponentId, String color) {
        this.diagramCanvas = diagramCanvas;
        this.diagramComponent = diagramCanvas.getComponent(diagramComponentId);

        this.newColor = color;

        this.oldColor = this.diagramComponent.getColor();
    }

    @Override
    public void execute() {
        System.out.println(this.newColor);
        diagramComponent.setColor(this.newColor);
        System.out.println(this.diagramComponent);
    }

    @Override
    public void undo() {
        diagramComponent.setColor(this.oldColor);
    }

    @Override
    public String toString() {
        return "Changes color for component " +
               this.diagramComponent.getText() +
               " from " +
               this.oldColor +
               " to " +
               this.newColor;
    }
}
