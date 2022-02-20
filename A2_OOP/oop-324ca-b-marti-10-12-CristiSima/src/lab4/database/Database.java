package lab4.database;

import lab4.people.Student;
import lab4.people.Teacher;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;

public class Database {
	final private List<Student> students = new ArrayList<>();
	final private List<Teacher> teachers = new ArrayList<>();

	private static int instances = 0;
	private static Database instance = null;

	static
	{
		instance = new Database();
	}

	public static Database getDatabase() {
		Database.instances++;
		return instance;
	}

	public static int getNumberOfInstances() {
		return Database.instances;
	}

	public void addTeachers(List<Teacher> teachers) {
		this.teachers.addAll(teachers);
	}

	public void addStudents(List<Student> students) {
		this.students.addAll(students);
	}

	public List<Teacher> getTeachersBySubject(String subject) {
		List<Teacher> teachers = new ArrayList<Teacher>();
		for (Teacher teacher:this.teachers) {
			if (teacher.getSubjects().contains(subject)) {
				teachers.add(teacher);
			}
		}
		return teachers;
	}

	public List<Student> getAllStudents() {
		return this.students;
	}

	public List<Teacher> getAllTeachers() {
		return this.teachers;
	}

	public List<Student> getStudentsBySubject(String subject) {
		List<Student> students = new ArrayList<Student>();
		for (Student student:this.students) {
			if (student.getSubjects().containsKey(subject)) {
				students.add(student);
			}
		}
		return students;
	}

	public List<Student> getStudentsByAverageGrade() {
		List<Student> students = new ArrayList<Student>();

		students.sort(new Student.comparatorAverageGrade());

		return students;
	}

	public List<Student> getStudentsByGradeForSubject(String subject) {
		List<Student> students = this.getStudentsBySubject(subject);

		students.sort(new Student.comparatorSubjectGrade(subject));

		return students;
	}
}
