package lab5.task2;

import lab5.task1.Task;

public class Queue extends ListContainer {
    
	public Task pop() {
		return this.list.remove(0);
	}

	public void push(Task task) {
		this.list.add(task);
	}
}
