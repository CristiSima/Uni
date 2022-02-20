package lab11.task1;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.util.ArrayList;
import java.util.Arrays;

public class TestMultiMapValue {
    private MultiMapValue<String, String> map;

    @BeforeEach
    public void setup() {
        map = new MultiMapValue<>();
    }

    @AfterEach
    public void clean() {
        map = null;
    }

    @Test
    public void testAddElementWithOneValue() {
        map.add("key", "value");
        Assertions.assertFalse(map.isEmpty());
        Assertions.assertEquals(1, map.size());
        Assertions.assertEquals(1, map.getValues("key").size());
    }

    @Test
    public void testAddElementWithMoreValues() {
        map.add("key", "value1");
        map.add("key", "value2");
        Assertions.assertFalse(map.isEmpty());
        Assertions.assertEquals(1, map.size());
        Assertions.assertEquals(2, map.getValues("key").size());
    }

    @Test
    public void testAddTwoElements() {
        map.add("key1", "value1");
        map.add("key2", "value2");
        Assertions.assertFalse(map.isEmpty());
        Assertions.assertEquals(2, map.size());
        Assertions.assertEquals(1, map.getValues("key1").size());
        Assertions.assertEquals(1, map.getValues("key2").size());
    }

    @Test
    public void testGetFirst() {
        map.add("key", "value");
        Assertions.assertEquals("value", map.getFirst("key"));
    }

    @Test
    public void testContainsKey() {
        map.add("key", "value");
        Assertions.assertTrue(map.containsKey("key"));
    }

    @Test
    public void testSize() {
        map.add("key1", "value1");
        Assertions.assertEquals(1, map.size());
        map.add("key2", "value2");
        Assertions.assertEquals(2, map.size());
        map.add("key3", "value3");
        Assertions.assertEquals(3, map.size());
    }

    @Test
    public void testRemoveKey() {
        map.add("key1", "value1");
        map.remove("key1");
        Assertions.assertEquals(0, map.size());
        Assertions.assertFalse(map.containsKey("key1"));
    }

    @Test
    public void testAddAllWithList() {
        List<String> list=new ArrayList();
        list.add("value1");
        list.add("value2");
        map.addAll("key", list);
        Assertions.assertEquals(1, map.size());
        Assertions.assertTrue(map.containsKey("key"));
        Assertions.assertEquals(2, map.getValues("key").size());
    }

    @Test
    public void testAddAllWithMultiMapValue() {
        MultiMapValue<String, String> newMap= new MultiMapValue();
        newMap.add("key1", "value1");
        newMap.add("key1", "value2");
        newMap.add("key2", "value2");
        map.addAll(newMap);
        Assertions.assertEquals(2, map.size());
        Assertions.assertTrue(map.containsKey("key1"));
        Assertions.assertTrue(map.containsKey("key2"));
        Assertions.assertEquals(2, map.getValues("key1").size());
        Assertions.assertEquals(1, map.getValues("key2").size());
    }
}
