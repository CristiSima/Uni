package lab7.task1.document;

/**
 * Represents a text segment of a document that needs to be parsed.
 */
public abstract class TextSegment {
	private String content;

	public TextSegment() {
		this.content = "";
	}

	public TextSegment(String content) {
		this.content = content;
	}

	public String getContent() {
		return content;
	}

	public void accept(DocumentVisitor visitor) {
		// visitor.visit(this);
	}
}
