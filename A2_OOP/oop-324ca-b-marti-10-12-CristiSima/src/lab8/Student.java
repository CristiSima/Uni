package lab8;

import java.lang.Comparable;
// import java.util.Comparator;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.HashMap;
import java.util.Collections;
import java.util.LinkedHashSet;
import java.util.HashSet;
import java.util.PriorityQueue;

public class Student implements Comparable<Student>
{
    private String name;
    private String surname;
    private long id;
    private double averageGrade;


    public Student(String name, String surname, long id ,double averageGrade)
    {
        this.name=name;
        this.surname=surname;
        this.id=id;
        this.averageGrade=averageGrade;
    }

    public int compareTo(Student student)
    {
        if(this.averageGrade!=student.averageGrade)
        {
            return Double.compare(this.averageGrade, student.averageGrade);
        }
        if(this.name.compareTo(student.name)!=0)
        {
            return this.name.compareTo(student.name);
        }
        return this.surname.compareTo(student.surname);
    }

    // public static class AscComparator extends Comparator

    public String toString()
    {
        return  "Student: {\n"+
                "\tname:\""+this.name+"\",\n"+
                "\tsurname:\""+this.surname+"\",\n"+
                "\tid:"+this.id+",\n"+
                "\taverageGrade:"+this.averageGrade+"\n"+
                "}";
    }

    // @Override
    public boolean equals(Student student)
    {
        return  this.name.equals(student.name) &&
                this.surname.equals(student.surname) &&
                this.id==student.id &&
                this.averageGrade==student.averageGrade;
    }

    @Override
    public int hashCode()
    {
        return (int) this.id *
                this.name.hashCode() *
                this.surname.hashCode() *
                (int)(this.averageGrade * 100f);
    }

    public static class Ex7Abomination extends LinkedHashSet<Integer>
    {
        // @Override   // dc se supara cand ii puneam Override
        public boolean add(int element)
        {
            if(element%2==0)
                return super.add(element);
            return false;
        }
    }

    public static class Ex7Abomination2 extends HashSet<Integer>
    {
        // @Override   // dc se supara cand ii puneam Override
        public boolean add(int element)
        {
            if(element%2==0)
                return super.add(element);
            return false;
        }
    }

    public static void main(String[] args) {
        ArrayList<Student> students=new ArrayList();
        students.add(new Student("N2","N1",68,5));
        students.add(new Student("N1","N1",67,5));
        students.add(new Student("N1","N",64,4.2));
        students.add(new Student("N2","N",65,4.2));
        students.add(new Student("N2","N1",66,4.2));

        Collections.sort(students);
        System.out.println(students);

        students.sort((Student student1, Student student2) -> -Double.compare(student1.averageGrade,student2.averageGrade));
        System.out.println(students);

        PriorityQueue<Student> students_pq=new PriorityQueue<Student>((Student student1, Student student2) -> Long.compare(student1.id,student2.id));
        students_pq.addAll(students);
        System.out.println("\tPQ");
        System.out.println(students_pq.poll());
        System.out.println(students_pq.poll());
        System.out.println(students_pq.poll());
        System.out.println(students_pq.poll());
        System.out.println(students_pq.poll());

        System.out.println(students.get(0).hashCode());

        HashMap<Student,LinkedList<String>> subjects=new HashMap();
        subjects.put(students.get(0),new LinkedList());
        subjects.get(students.get(0)).add("yes");
        subjects.put(students.get(1),new LinkedList());
        subjects.get(students.get(1)).add("no");
        subjects.put(students.get(2),new LinkedList());
        subjects.get(students.get(2)).add("pz");

        for(HashMap.Entry<Student,LinkedList<String>> entry: subjects.entrySet())
        {
            System.out.println(entry.getKey().name+" "+entry.getKey().surname+":"+entry.getValue());
        }

        Ex7Abomination part1=new Ex7Abomination();
        part1.add(4);
        part1.add(2);
        part1.add(3);
        part1.add(2);
        part1.add(6);
        part1.forEach((nr) -> System.out.print(nr+", "));
        System.out.println();

        Ex7Abomination2 part2=new Ex7Abomination2();
        part2.add(4);
        part2.add(2);
        part2.add(3);
        part2.add(2);
        part2.add(6);
        part2.forEach((nr) -> System.out.print(nr+", "));
        System.out.println();
        // HashSet se sorteaza

        Student s111=new Student("N1","S1",1,1);

        Student s112=new Student("N1","S1",1,2);

        System.out.println(s111.compareTo(s112));
    }
}
