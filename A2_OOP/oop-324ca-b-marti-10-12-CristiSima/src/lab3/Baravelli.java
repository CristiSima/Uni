import java.lang.Math;

class Baravelli extends CandyBox {
	private float radius, height;
	public Baravelli() {
		this.radius = 0;
		this.height = 0;
	}
	public Baravelli(float radius, float height) {
		super("the welsh one", "Wales");
		this.radius = radius;
		this.height = height;
	}

	@Override
	public float getVolume() {
		return (float)Math.PI * (float)Math.pow(this.radius, 2) * this.height;
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
			return this.radius == ((Baravelli)other).radius &&
			       this.height == ((Baravelli)other).height;
		} else {
			return false;
		}
	}

	@Override
	public void printDim() {
		// old printBaravelliDim
		String json = "{\n";

		json += "\t\"content\":\"Baravelli dimensions\",\n";
		json += "\t\"radius\":" + this.radius + ",\n";
		json += "\t\"height\":" + this.height + ",\n";
		json += "}";

		System.out.println(json);
	}

	public static void main(String[] args) {
		CandyBox box0 = new Baravelli();
		System.out.println(box0);

		CandyBox boxU = new Baravelli(2, 3);
		CandyBox b0xU = new Baravelli(2, 3);
		
		System.out.println(boxU);
		((Baravelli)boxU).printDim();

		System.out.println(box0.equals(boxU));
		System.out.println(b0xU.equals(boxU));
	}
}
