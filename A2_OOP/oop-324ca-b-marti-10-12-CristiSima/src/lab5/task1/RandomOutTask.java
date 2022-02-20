package lab5.task1;

import java.util.Random;

public class RandomOutTask implements Task {
	private int Finish_num;

	public RandomOutTask() {
		Random rand = new Random();
		this.Finish_num = rand.nextInt(1024);
	}

	public void execute() {
		System.out.println(this.Finish_num);
	}
}
