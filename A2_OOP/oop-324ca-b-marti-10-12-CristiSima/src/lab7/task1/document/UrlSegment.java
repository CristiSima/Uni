package lab7.task1.document;

public class UrlSegment extends TextSegment {
	private String url;
	private String description;

	public UrlSegment(String url, String description) {
		this.url = url;
		this.description = description;
	}

	@Override
	public void accept(DocumentVisitor visitor) {
		visitor.visit(this);
	}

	public String getUrl() {
		return url;
	}

	public String getDescription() {
		return description;
	}
}
