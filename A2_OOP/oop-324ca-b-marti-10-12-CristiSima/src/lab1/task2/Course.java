import java.util.*;

public class Course {
	private String title;
	private String description;
	private ArrayList<Student> students;

	public Course(String title, String description) {
		this.title = title;
		this.description = description;
		this.students = new ArrayList<Student>();
	}

	public void register_student(Student student) {
		this.students.add(student);

		if (student.get_year() < 1)
			System.out.println("Vine un nestudent la cursu de " + this.title);
	}

	public int print_from_year(int year) {
		int printed = 0;

		for (Student student:this.students)
			if (student.get_year() == year) {
				student.print();
				printed++;
			}

		return printed;
	}

	public ArrayList<Student> filterYear(int year) {
		ArrayList<Student> filtered_students = new ArrayList<Student>();

		for (Student student:this.students)
			if (student.get_year() == year)
				filtered_students.add(student);

		return filtered_students;
	}
}
