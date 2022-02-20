class CandyBox {
	private String flavor;
	private String origin;

	public CandyBox() {
		this.flavor = "";
		this.origin = "";
	}
	public CandyBox(String flavor, String origin) {
		this.flavor = flavor;
		this.origin = origin;
	}

	public float getVolume() {
		return 0;
	}

	public String toString() {
		String json = "{\n";

		json += "\t\"flavor\":\"" + this.flavor + "\",\n";
		json += "\t\"origin\":\"" + this.origin + "\"\n";
		json += "}";

		return json;
	}

	public boolean equals(CandyBox other) {
		if ( this.getClass() == other.getClass() &&
		     this.flavor == other.flavor &&
		     this.origin == other.origin) {
			// if this is a subclass this is supposed to
			// be called by the equals in that subclass
			return true;
		} else {
			return false;
		}
	}

	public void printDim() {
		String json = "{\n";

		json += "\t\"content\":\"Unknown\"\n";
		json += "}";

		System.out.println(json);
	}

	public static void main(String[] args) {
		CandyBox box0 = new CandyBox();
		System.out.println(box0);

		CandyBox boxU = new CandyBox("sweet?idk it's candy", "a local producer they say");
		System.out.println(boxU);

		System.out.println(new CandyBox().equals(new Baravelli()));
		System.out.println(new CandyBox().equals(2));
	}
}
