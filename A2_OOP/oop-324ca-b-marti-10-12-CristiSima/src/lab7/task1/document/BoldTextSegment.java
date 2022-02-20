package lab7.task1.document;

public class BoldTextSegment extends TextSegment {

	public BoldTextSegment(String content) {
		super(content);
	}

	@Override
	public void accept(DocumentVisitor visitor) {
		visitor.visit(this);
	}
}
