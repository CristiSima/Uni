class Area {
	private CandyBag bag;
	private int number;
	private String street;

	public Area() {
		this.bag = null;
		this.number = 0;
		this.street = "";
	}

	public Area(CandyBag bag, int number, String street) {
		this.bag = bag;
		this.number = number;
		this.street = street;
	}

	public void getBirthdayCard() {
		System.out.println(this.street + ", Nr. " + this.number);
		System.out.println("La multi ani!!");

		String json = "[\n";

		for (CandyBox box:this.bag.getBoxes()) {
			json += box.toString() + ",\n";
		}

		json = json.substring(0, json.length() - 2);
		json = json.replace("\n", "\n\t");
		json += "\n]";

		System.out.println(json);
	}
	public static void main(String[] args) {
		CandyBag bag = new CandyBag();

		bag.add(new Baravelli(3, 10));
		bag.add(new ChocAmor(5));

		Area A = new Area(bag, 34, "Mihai Bravu");

		A.getBirthdayCard();
		for (CandyBox box:A.bag.getBoxes()) {
			box.printDim();
		}
	}
}
