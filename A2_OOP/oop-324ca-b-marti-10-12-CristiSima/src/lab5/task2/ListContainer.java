package lab5.task2;

import lab5.task1.Task;
import java.util.ArrayList;

abstract class ListContainer implements Container {
	protected ArrayList<Task> list;

	public ListContainer() {
		this.list = new ArrayList<Task>();
	}

	public int size() {
		return this.list.size();
	}

	public boolean isEmpty() {
		return this.list.isEmpty();
	}

	public void transferFrom(Container container) {
		while (!container.isEmpty()) {
			this.push(container.pop());
		}
	}
}
