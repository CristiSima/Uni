package lab7.task1.document;

public interface DocumentVisitor {
	public void visit(ItalicTextSegment segment);
	public void visit(BoldTextSegment segment);
	public void visit(UrlSegment segment);
	public void visit(PlainTextSegment segment);
	public StringBuilder getDocument();
}
