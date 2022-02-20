package lab7.task1.document;

public class PlainTextSegment extends TextSegment {

	public PlainTextSegment(String content) {
		super(content);
	}

	@Override
	public void accept(DocumentVisitor visitor) {
		visitor.visit(this);
	}
}
