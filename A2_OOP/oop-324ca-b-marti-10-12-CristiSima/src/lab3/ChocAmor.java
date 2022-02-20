import java.lang.Math;

class ChocAmor extends CandyBox {
	private float lenght;
	public ChocAmor() {
		this.lenght = 0;
	}
	public ChocAmor(float lenght) {
		super("new", "Colombia");
		this.lenght = lenght;
	}

	@Override
	public float getVolume() {
		return (float)Math.pow(this.lenght, 3);
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
			return this.lenght == ((ChocAmor)other).lenght;
		} else {
			return false;
		}
	}

	@Override
	public void printDim() {
		// old printChocAmorDim
		String json = "{\n";

		json += "\t\"content\":\"Choc Amor dimensions\",\n";
		json += "\t\"lenght\":" + this.lenght + ",\n";
		json += "}";

		System.out.println(json);
	}

	public static void main(String[] args) {
		CandyBox box0 = new ChocAmor();
		System.out.println(box0);

		CandyBox boxU = new ChocAmor(4);
		CandyBox b0xU = new ChocAmor(4);

		System.out.println(boxU);
		((ChocAmor)boxU).printDim();

		System.out.println(box0.equals(boxU));
		System.out.println(b0xU.equals(boxU));
	}
}
