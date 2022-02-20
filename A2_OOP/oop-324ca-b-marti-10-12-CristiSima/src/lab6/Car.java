package lab6;

class Car {
	static public enum CarType
	{
		DeLorean,
		Dacia_1310,
		Logan_2007
	}
	private int price;
	private int fabricationYear;
	private CarType type;

	public Car(CarType type, int fabricationYear, int price) {
		this.price = price;
		this.fabricationYear = fabricationYear;
		this.type = type;
	}

	public Car.CarType getType() {
		return this.type;
	}

	public int getPrice() {
		return this.price;
	}

	public int getFabricationYear() {
		return this.fabricationYear;
	}

}
