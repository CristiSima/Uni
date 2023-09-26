package task6;


import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.RecursiveTask;

class MyTask extends RecursiveTask<Void> {
    int[] graph;
    int step;

    public MyTask(int[] graph, int step) {
        this.graph = graph;
        this.step = step;
    }

    @Override
    protected Void compute() {
        // System.out.println("IN");
        if (Main.N == step) {
            Main.printQueens(graph);
            return null;
        }

        List<MyTask> tasks = new ArrayList<>();
        for (int i = 0; i < Main.N; ++i) {
            int[] newGraph = graph.clone();
            newGraph[step] = i;


            if(Main.check(newGraph, step))
            {
                MyTask t = new MyTask(newGraph, step + 1);
                tasks.add(t);
                t.fork();
            }
        }

        for (MyTask task : tasks) {
            task.join();
        }
        return null;

    }
}

class MyTask2 extends RecursiveTask<Void> {
    int[] graph;
    int step;

    public MyTask2(int[] graph, int step) {
        this.graph = graph;
        this.step = step;
    }

    @Override
    protected Void compute() {
        // System.out.println("IN");
        if (Main.N == step) {
            Main.printQueens(graph);
            return null;
        }
        if(! Main.check(graph, step-1))
        {
            // System.out.println("DEAD");
            return null;
        }

        List<MyTask2> tasks = new ArrayList<>();
        for (int i = 0; i < Main.N; ++i) {
            int[] newGraph = graph.clone();
            newGraph[step] = i;

            MyTask2 t = new MyTask2(newGraph, step + 1);
            tasks.add(t);
            t.fork();
        }

        for (MyTask2 task : tasks) {
            task.join();
        }
        return null;

    }
}


public class Main {
    public static int N = 10;

    public static boolean check(int[] arr, int step) {
        for (int i = 0; i <= step; i++) {
            for (int j = i + 1; j <= step; j++) {
                if (arr[i] == arr[j] || arr[i] + i == arr[j] + j || arr[i] + j == arr[j] + i)
                    return false;
            }
        }
        return true;
    }

    public static void printQueens(int[] sol) {
        StringBuilder aux = new StringBuilder();
        for (int i = 0; i < sol.length; i++) {
            aux.append("(").append(sol[i] + 1).append(", ").append(i + 1).append("), ");
        }
        aux = new StringBuilder(aux.substring(0, aux.length() - 2));
        System.out.println("[" + aux + "]");
    }

    public static void queens(int[] graph, int step) {
        if (Main.N == step) {
            printQueens(graph);
            return;
        }
        for (int i = 0; i < Main.N; ++i) {
            int[] newGraph = graph.clone();
            newGraph[step] = i;

            if (check(newGraph, step)) {
                queens(newGraph, step + 1);
            }
        }
    }
    public static void main(String[] args) {
        int[] graph = new int[N];
        queens(graph, 0);

        ForkJoinPool fjp = new ForkJoinPool(2);
        // fjp.invoke(new MyTask(graph, 0));
        // fjp.invoke(new MyTask2(graph, 0));
        fjp.shutdown();
    }
}
