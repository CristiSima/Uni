package lab11.task2;

import java.util.Vector;

class MyVector3 implements Summable {
    private Vector<Integer> vector;

    public MyVector3() {
        this.vector = new Vector(3);


        for (int i = 0; i < 3; i++) {
            this.vector.add(0);
        }
    }

    public int get(int x) {
        if (0 <= x && x < 3) {
            return this.vector.get(x);
        }
        return 0;
    }

    public void set(int x, int value) {
        if (0 <= x && x < 3) {
            this.vector.setElementAt(value, x);
        }
    }

    @Override
    public void addValue(Summable value) {
        if (!(value instanceof MyVector3)) {
            return;
        }
        MyVector3 other = (MyVector3)value;
        for (int i = 0; i < 3; i++) {
            this.set(i, this.get(i) + other.get(i));
        }
    }

    @Override
    public String toString() {
        return this.vector.toString();
    }
}
