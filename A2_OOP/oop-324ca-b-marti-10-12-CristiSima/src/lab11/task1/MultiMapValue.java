package lab11.task1;

import java.util.*;

public class MultiMapValue<K, V> {
    HashMap<K, ArrayList<V> > lists;

    MultiMapValue() {
        this.lists = new HashMap();
    }

    public void add(K key, V value) {
        if (!this.containsKey(key)) {
            this.lists.put(key, new ArrayList());
        }
        this.lists.get(key).add(value);
    }

    public void addAll(K key, List<V> values) {
        if (!this.containsKey(key)) {
            this.lists.put(key, new ArrayList());
        }
        this.lists.get(key).addAll(values);
    }

    public void addAll(MultiMapValue<K, V> map) {
        for (var entry: map.lists.entrySet()) {
            this.addAll(entry.getKey(), entry.getValue());
        }
    }

    public V getFirst(K key) {
        if (!this.lists.containsKey(key)) {
            return null;
        }
        return this.lists.get(key).get(0);
    }

    public List<V> getValues(K key) {
        if (!this.lists.containsKey(key)) {
            return null;
        }
        return this.lists.get(key);
    }

    public boolean containsKey(K key) {
        return this.lists.containsKey(key);
    }

    public boolean isEmpty() {
        return this.lists.size() == 0;
    }

    public List<V> remove(K key) {
        if (!this.lists.containsKey(key)) {
            return null;
        }
        return this.lists.remove(key);
    }

    public int size() {
        return this.lists.size();
    }
}
