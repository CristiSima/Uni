package philosophersProblem;

public class Philosopher implements Runnable {
    private final Object leftFork;
    private final Object rightFork;
    private final int id;

    public Philosopher(int id, Object leftFork, Object rightFork) {
        this.leftFork = leftFork;
        this.rightFork = rightFork;
        this.id = id;
    }

    private void sleep() {
        try {
            Thread.sleep(100);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void run() {
        Object firstFork =(id%2==0)?leftFork:rightFork;
        Object secondFork=(id%2==1)?leftFork:rightFork;
        synchronized (firstFork) {
            sleep(); // delay added to make sure the dead-lock is visible
            synchronized (secondFork) {
                System.out.println("Philosopher " + id + " is eating");
            }
        }
    }
}
