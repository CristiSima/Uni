package lab11.task2;

import java.util.Vector;

class MyMatrix implements Summable {
    private Vector<Vector<Integer> > matrix;

    public MyMatrix() {
        this.matrix = new Vector(4);
        this.matrix.add(new Vector(4));
        this.matrix.add(new Vector(4));
        this.matrix.add(new Vector(4));
        this.matrix.add(new Vector(4));

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++)                         {
                this.matrix.get(i).add(0);
            }
        }
    }

    public int get(int x, int y) {
        if (0 <= x && x < 4 && 0 <= y && y < 4) {
            return this.matrix.get(x).get(y);
        }
        return 0;
    }

    public void set(int x, int y, int value) {
        if (0 <= x && x < 4 && 0 <= y && y < 4) {
            this.matrix.get(x).setElementAt(value, y);
        }
    }

    @Override
    public void addValue(Summable value) {
        if (!(value instanceof MyMatrix)) {
            return;
        }
        MyMatrix other = (MyMatrix) value;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++)                         {
                this.set(i, j, this.get(i, j) + other.get(i, j));
            }
        }
    }

    @Override
    public String toString() {
        return this.matrix.get(0).toString() + "\n" +
               this.matrix.get(1).toString() + "\n" +
               this.matrix.get(2).toString() + "\n" +
               this.matrix.get(3).toString();
    }
}
