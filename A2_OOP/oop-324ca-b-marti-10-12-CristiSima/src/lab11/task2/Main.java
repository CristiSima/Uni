package lab11.task2;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;

public class Main {

    public static <T extends Summable> T sumSumabil(Collection<? extends Summable> list, T dest) {
        for (Summable obj: list) {
            dest.addValue(obj);
        }
        return dest;
    }

    public static void main(final String[] args) {
        MyMatrix m1 = new MyMatrix();
        MyMatrix m2 = new MyMatrix();
        MyVector3 v1 = new MyVector3();
        MyVector3 v2 = new MyVector3();

        m1.set(1, 2, 3);
        m2.set(2, 2, 2);
        v1.set(1, 1);
        v2.set(0, 99);

        ArrayList<MyMatrix> list1 = new ArrayList();
        list1.add(m1);
        list1.add(m2);
        ArrayList<MyVector3> list2 = new ArrayList();
        list2.add(v1);
        list2.add(v2);
        ArrayList<Summable> list3 = new ArrayList();
        list3.add(m1);
        list3.add(m2);
        list3.add(v1);
        list3.add(v2);

        System.out.println(Main.< MyMatrix > sumSumabil(list1, new MyMatrix()));
        System.out.println(Main.< MyVector3 > sumSumabil(list1, new MyVector3()));

        System.out.println(Main.< MyMatrix > sumSumabil(list2, new MyMatrix()));
        System.out.println(Main.< MyVector3 > sumSumabil(list2, new MyVector3()));

        System.out.println(Main.< MyMatrix > sumSumabil(list3, new MyMatrix()));
        System.out.println(Main.< MyVector3 > sumSumabil(list3, new MyVector3()));
    }
}
