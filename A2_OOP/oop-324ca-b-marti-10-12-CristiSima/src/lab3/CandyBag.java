import java.util.ArrayList;
class CandyBag {
	private ArrayList<CandyBox> boxes;

	public CandyBag() {
		this.boxes = new ArrayList<CandyBox>();
	}

	public ArrayList<CandyBox> getBoxes() {
		return this.boxes;
	}

	public void add(CandyBox box) {
		this.boxes.add(box);
	}
}
