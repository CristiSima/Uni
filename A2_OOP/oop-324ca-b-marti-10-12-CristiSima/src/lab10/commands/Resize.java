package lab10.commands;

import lab10.diagram.*;

public class Resize implements DrawCommand {
    private DiagramCanvas diagramCanvas;
    private DiagramComponent diagramComponent;

    private int oldHeight;
    private int newHeight;
    private int oldWidth;
    private int newWidth;

    public Resize(DiagramCanvas diagramCanvas, int diagramComponentId, int newHeight, int newWidth) {
        this.diagramCanvas = diagramCanvas;
        this.diagramComponent = diagramCanvas.getComponent(diagramComponentId);

        this.newHeight = newHeight;
        this.newWidth = newWidth;

        this.oldWidth = this.diagramComponent.getWeight();
        this.oldHeight = this.diagramComponent.getHeight();
    }

    @Override
    public void execute() {
        diagramComponent.setHeight(this.newHeight);
        diagramComponent.setWeight(this.newWidth);
    }

    @Override
    public void undo() {
        diagramComponent.setHeight(this.oldHeight);
        diagramComponent.setWeight(this.oldWidth);
    }

    @Override
    public String toString() {
        return "Resizes component " +
               this.diagramComponent.getText() +
               " from (" +
               this.oldHeight + "," + this.oldWidth +
               ") to (" +
               this.newHeight + "," + this.newWidth +
               ")";
    }
}
