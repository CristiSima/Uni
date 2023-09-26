package multipleProducersMultipleConsumersNBuffer;
import java.util.concurrent.Semaphore;

import java.util.Queue;

public class Buffer {

    Queue<Integer> queue;
    private Semaphore empty;
    private Semaphore full;

    public Buffer(int size) {
        queue = new LimitedQueue<>(size);
        empty=new Semaphore(size);
        full=new Semaphore(0);
    }

    void put(int value) {
        try {
            empty.acquire();
        } catch (Exception e) {

        }
        synchronized (this)
        {
            queue.add(value);
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
            value=queue.poll();
        }
        empty.release();
        return value;
    }
}
