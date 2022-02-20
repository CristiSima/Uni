class Point {
	private float x, y;

	public Point(float x, float y) {
		this.x = x;
		this.y = y;
	}

	public void changeCoords(float x, float y) {
		this.x = x;
		this.y = y;
	}

	@Override
	public String toString() {
		return String.format("(%f, %f)", x, y);
	}
}
