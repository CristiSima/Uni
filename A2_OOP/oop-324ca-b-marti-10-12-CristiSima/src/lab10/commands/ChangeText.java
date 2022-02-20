package lab10.commands;

import lab10.diagram.*;

public class ChangeText implements DrawCommand {
    private DiagramCanvas diagramCanvas;
    private DiagramComponent diagramComponent;

    private String oldText;
    private String newText;

    public ChangeText(DiagramCanvas diagramCanvas, int diagramComponentId, String text) {
        this.diagramCanvas = diagramCanvas;
        this.diagramComponent = diagramCanvas.getComponent(diagramComponentId);

        this.newText = text;

        this.oldText = this.diagramComponent.getText();
    }

    @Override
    public void execute() {
        diagramComponent.setText(this.newText);
    }

    @Override
    public void undo() {
        diagramComponent.setText(this.oldText);
    }

    @Override
    public String toString() {
        return "Changes text for component " +
               this.diagramComponent.getText() +
               " from " +
               "\"" + this.oldText + "\"" +
               " to " +
               "\"" + this.newText + "\"";
    }
}
