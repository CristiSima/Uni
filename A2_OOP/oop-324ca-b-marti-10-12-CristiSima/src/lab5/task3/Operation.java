package lab5.task3;

public class Operation implements Minus, Plus, Mult, Div {
	private float number;

	public Operation(float number) {
		this.number = number;
	}

	public void minus(float number) {
		this.number -= number;
	}

	public void plus(float number) {
		this.number += number;
	}

	public void mult(float number) {
		this.number *= number;
	}

	public void div(float number) {
		if (number == 0) {
			return;
		}
		this.number /= number;
	}

	public int getNumber() {
		return (int)this.number;
	}
}
