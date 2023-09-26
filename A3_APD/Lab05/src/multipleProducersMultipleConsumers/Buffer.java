package multipleProducersMultipleConsumers;
import java.util.concurrent.Semaphore;

public class Buffer {
    private int a;
    private Semaphore empty;
    private Semaphore full;

    public Buffer()
    {
        empty=new Semaphore(1);
        full=new Semaphore(0);
    }

    void put(int value) {
        try {
            empty.acquire();
        } catch (Exception e) {

        }
        synchronized (this)
        {
            a = value;
        }
        full.release();
    }

    int get() {
        try {
            full.acquire();
        } catch (Exception e) {

        }
        int value;
        synchronized (this)
        {
            value=a;
        }
        empty.release();
        return value;
    }
}
