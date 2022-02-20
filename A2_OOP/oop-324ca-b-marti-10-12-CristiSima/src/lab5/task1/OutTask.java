package lab5.task1;

public class OutTask implements Task {
	private String Finish_msg;

	public OutTask(String Finish_msg) {
		this.Finish_msg = new String(Finish_msg);
	}

	public void execute() {
		System.out.println(this.Finish_msg);
	}
}
