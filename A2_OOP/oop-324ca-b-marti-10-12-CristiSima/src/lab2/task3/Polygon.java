import java.util.*;

class Polygon {
	private int n;
	private Vector<Point> points;

	public Polygon(int n) {
		this.n = n;
		this.points = new Vector<Point>(n);
	}

	public Polygon(float coords[]) {
		this(coords.length / 2);
		for (int i = 0; i < n; i++) {
			this.points.add(new Point(coords[i << 1], coords[i << 1 | 1]));
		}
	}

	@Override
	public String toString() {
		String json = "{\n";

		if (!this.points.isEmpty()) {
			for (Point point: this.points) {
				json += "\t" + point.toString() + ",\n";
			}
		} else {
			json += "\tNull\n";
		}

		json += "}";

		return json;
	}

	public static void main(String[] args) {
		Polygon impossible = new Polygon(2);
		System.out.println(impossible);

		float coords[] = {0, 0, 1, 0, 1, 1, 0, 1};
		Polygon square = new Polygon(coords);
		System.out.println(square);
	}
}
