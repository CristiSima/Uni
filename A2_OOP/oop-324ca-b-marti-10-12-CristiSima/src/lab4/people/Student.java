package lab4.people;

import lab4.people.Teacher;

import lab4.database.Database;

import java.util.Comparator;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public class Student {
	private String firstName;
	private String lastName;
	private Map<String, Integer> subjects;

	public Student(String firstName, String lastName, Map<String, Integer> subjects) {
		this.firstName = firstName;
		this.lastName = lastName;
		this.subjects = subjects;
	}

	public Student(Student other) {
		this.firstName = new String(other.firstName);
		this.lastName = new String(other.lastName);
		this.subjects = new HashMap<String, Integer>(other.subjects);
	}

	public String getFirstName() {
		return firstName;
	}

	public void setFirstName(String firstName) {
		this.firstName = firstName;
	}

	public String getLastName() {
		return lastName;
	}

	public void setLastName(String lastName) {
		this.lastName = lastName;
	}

	public Map<String, Integer> getSubjects() {
		return subjects;
	}

	public void setSubjects(HashMap<String, Integer> subjects) {
		this.subjects = subjects;
	}

	public double averageGrade() {
		double average = 0;

		for (Integer grade:this.subjects.values()) {
			average += grade;
		}

		return average / this.subjects.size();
	}

	public List<Teacher> getAllTeachers() {
		List<Teacher> copyList = new ArrayList<Teacher>();
		for (Teacher teacher:Database.getDatabase().getAllTeachers()) {
			copyList.add(new Teacher(teacher));
		}

		return Collections.unmodifiableList(copyList);
	}

	public int getGradeForSubject(String subject) {
		return this.subjects.get(subject);
	}

	@Override
	public String toString() {
		return "Student: " + firstName + " " + lastName + "\n"
		       + "Subjects: " + subjects + "\n";
	}

	public List<Teacher> getTeachersBySubject(String subject) {
		List<Teacher> copyList = new ArrayList<Teacher>();
		for (Teacher teacher:Database.getDatabase().getTeachersBySubject(subject)) {
			copyList.add(new Teacher(teacher));
		}

		return Collections.unmodifiableList(copyList);
	}

	public List<Student> getAllStudents() {
		List<Student> copyList = new ArrayList<Student>();
		for (Student student:Database.getDatabase().getAllStudents()) {
			copyList.add(new Student(student));
		}

		return Collections.unmodifiableList(copyList);
	}

	public List<Student> getStudentsBySubject(String subject) {
		List<Student> copyList = new ArrayList<Student>();
		for (Student student:Database.getDatabase().getStudentsBySubject(subject)) {
			copyList.add(new Student(student));
		}

		return Collections.unmodifiableList(copyList);
	}

	public List<Student> getStudentsByAverageGrade() {
		List<Student> copyList = new ArrayList<Student>();
		for (Student student:Database.getDatabase().getStudentsByAverageGrade()) {
			copyList.add(new Student(student));
		}

		return Collections.unmodifiableList(copyList);
	}

	public List<Student> getStudentsByGradeForSubject(String subject) {
		List<Student> copyList = new ArrayList<Student>();
		for (Student student:Database.getDatabase().getStudentsByGradeForSubject(subject)) {
			copyList.add(new Student(student));
		}

		return Collections.unmodifiableList(copyList);
	}

	public static class comparatorAverageGrade implements Comparator<Student> {
		@Override
		public int compare(Student student1, Student student2) {
			double average1 = student1.averageGrade();
			double average2 = student2.averageGrade();

			if (average1 > average2) {
				return -1;
			} else if (average1 < average2) {
				return 1;
			} else {
				return 0;
			}
		}
	}

	public static class comparatorSubjectGrade implements Comparator<Student> {
		private String subject;
		public comparatorSubjectGrade(String subject) {
			this.subject = subject;
		}

		@Override
		public int compare(Student student1, Student student2) {
			int grade1 = student1.getSubjects().get(this.subject);
			int grade2 = student2.getSubjects().get(this.subject);

			return grade1 - grade2;
		}
	}
}
