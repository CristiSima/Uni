public class Test {
	public static void main(String[] args) {
		int year = 0;

		if (args.length > 0) {
			try {
				year = Integer.parseInt(args[0]);
			} catch (NumberFormatException e) {
				System.out.println("First arg must be an int");
				System.exit(1);
			}
		}else {
			System.out.println("First arg must be an int");
			System.exit(1);
		}

		Course OOP = new Course("OOP", "OOP in java");

		OOP.register_student(new Student("Mihai", 2));
		OOP.register_student(new Student("Rusu", 2));
		OOP.register_student(new Student("Sorin", 2));
		OOP.register_student(new Student("Paul", 3));
		OOP.register_student(new Student("Andrei", 1));

		if (OOP.print_from_year(year) == 0)
			System.out.println("No students from year " + String.valueOf(year) + " here.");

		Student Sima = new Student("Sima", 2);
		Student Sim2a = new Student("Sima", 2);

		System.out.println("Sima == Sim2a ???" + Sima.equals(Sim2a)); // false
		System.out.println("Da dece??\n\tpt ca sunt instante diferite de clasa");
		System.out.println("\tchiar daca au aceleasi proprietati.");

		System.exit(0);
	}
}
