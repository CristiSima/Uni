public class Student {
	private String name;
	private int year;

	public Student(String name, int year) {
		this.name = name;
		this.year = year;

		if (year < 1)
			System.out.println("Avem un prestudent aici");
	}

	public int get_year() {
		return this.year;
	}

	public void print() {
		System.out.println("Student:   " + this.name);
		System.out.println("From year: " + this.year);
		System.out.println();
	}
}
