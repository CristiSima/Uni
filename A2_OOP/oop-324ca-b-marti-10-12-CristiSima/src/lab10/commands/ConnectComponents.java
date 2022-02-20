package lab10.commands;

import lab10.diagram.*;

public class ConnectComponents implements DrawCommand {
	private DiagramCanvas diagramCanvas;
	private int diagramComponent1Id;
	private int diagramComponent2Id;

	public ConnectComponents(DiagramCanvas diagramCanvas, int diagramComponent1Id, int diagramComponent2Id) {
		this.diagramCanvas = diagramCanvas;
		this.diagramComponent1Id = diagramComponent1Id;
		this.diagramComponent2Id = diagramComponent2Id;
	}

	@Override
	public void execute() {
		this.diagramCanvas.getComponent(diagramComponent1Id)
		.connectTo(diagramComponent2Id + "");
		this.diagramCanvas.getComponent(diagramComponent2Id)
		.connectTo(diagramComponent1Id + "");
	}

	@Override
	public void undo() {
		this.diagramCanvas.getComponent(diagramComponent1Id)
		.removeConnection(diagramComponent2Id + "");
		this.diagramCanvas.getComponent(diagramComponent2Id)
		.removeConnection(diagramComponent1Id + "");
	}

	@Override
	public String toString() {
		return "Connects component: " +
		       this.diagramComponent1Id +
		       " and " +
		       this.diagramComponent2Id;
	}
}
