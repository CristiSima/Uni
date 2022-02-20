class Lindt extends CandyBox {
	private float lenght, width, height;
	public Lindt() {
		this.lenght = 0;
		this.width = 0;
		this.height = 0;
	}
	public Lindt(float lenght, float width, float height) {
		super("truffles", "Switzerland");
		this.lenght = lenght;
		this.width = width;
		this.height = height;
	}

	@Override
	public float getVolume() {
		return this.lenght * this.width * this.height;
	}

	@Override
	public String toString() {
		String json = super.toString();
		json = json.substring(0, json.length() - 2) + ",\n";

		json += "\t\"volume\":" + this.getVolume() + "\n";
		json += "}";

		return json;
	}

	@Override
	public boolean equals(CandyBox other) {
		if (super.equals(other)) {
			return this.lenght == ((Lindt)other).lenght &&
			       this.width  == ((Lindt)other).width  &&
			       this.height == ((Lindt)other).height;
		} else {
			return false;
		}
	}

	@Override
	public void printDim() {
		// old printLindtDim
		String json = "{\n";

		json += "\t\"content\":\"Lindt dimensions\",\n";
		json += "\t\"lenght\":" + this.lenght + ",\n";
		json += "\t\"width\":" + this.width + ",\n";
		json += "\t\"height\":" + this.height + ",\n";
		json += "}";

		System.out.println(json);
	}

	public static void main(String[] args) {
		CandyBox box0 = new Lindt();
		System.out.println(box0);

		CandyBox boxU = new Lindt(2, 4, 8);
		CandyBox b0xU = new Lindt(2, 4, 8);

		System.out.println(boxU);
		((Lindt)boxU).printDim();

		System.out.println(box0.equals(boxU));
		System.out.println(b0xU.equals(boxU));
	}
}
