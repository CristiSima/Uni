package lab7.task1.document;

public class DokuWikiVisitor implements DocumentVisitor {
	private StringBuilder sb = new StringBuilder();

	public void visit(ItalicTextSegment segment) {
		this.sb.append("//");
		this.sb.append(segment.getContent());
		this.sb.append("//");
	}
	public void visit(BoldTextSegment segment) {
		this.sb.append("**");
		this.sb.append(segment.getContent());
		this.sb.append("**");
	}
	public void visit(UrlSegment segment) {
		this.sb.append("[[");
		this.sb.append(segment.getUrl());
		this.sb.append("|");
		this.sb.append(segment.getDescription());
		this.sb.append("]]");
	}
	public void visit(PlainTextSegment segment) {
		this.sb.append(segment.getContent());
	}
	public StringBuilder getDocument() {
		return new StringBuilder(this.sb);
	}

}
