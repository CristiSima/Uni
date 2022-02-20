package lab8;

import org.junit.jupiter.api.*;

public class StudentTest {
    // private Student student;

    @BeforeEach
    public void setUp() {
        // this.student = new Student();
    }

    @AfterEach
    public void clean() {
        // this.student = null;
    }

    @Test
    @DisplayName("toString Test")
    public void testToString() {
        Assertions.assertEquals(new Student("A","B",1,2.5).toString(),
                                "Student: {\n}"+
                                "\tname:\"A\",\n"+
                                "\tsurname:\"B\",\n"+
                                "\tid:1,\n"+
                                "\taverageGrade:2.5\n"+
                                "}");
    }

    @Test
    @DisplayName("compareTo Test")
    public void testCompareTo() {
        // variable meaning: name surname averageGrade
        // compare order should be: averageGrade name surname
        Student s111=new Student("N1","S1",1,1);
        Student s121=new Student("N1","S2",1,1);
        Student s211=new Student("N2","S1",1,1);
        Student s221=new Student("N2","S2",1,1);
        Student s112=new Student("N1","S1",1,2);
        Student s122=new Student("N1","S2",1,2);
        Student s212=new Student("N2","S1",1,2);
        Student s222=new Student("N2","S2",1,2);
        Assertions.assertEquals(0, s111.compareTo(s111));
        Assertions.assertTrue(0>s111.compareTo(s112));
        Assertions.assertTrue(0>s111.compareTo(s211));
        Assertions.assertTrue(0>s111.compareTo(s121));

        Assertions.assertTrue(0<s222.compareTo(s112));
        Assertions.assertTrue(0<s222.compareTo(s212));
        Assertions.assertTrue(0==s222.compareTo(s222));

        Assertions.assertTrue(0>s222.compareTo(s112));
        Assertions.assertTrue(0>s222.compareTo(s211));
        Assertions.assertTrue(0>s222.compareTo(s121));
    }
}
