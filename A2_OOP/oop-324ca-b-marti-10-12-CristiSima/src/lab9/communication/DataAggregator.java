package lab9.communication;

import lab9.storage.DataRepository;
import lab9.main.Utils;
import lab9.dataprocessing.StepCountStrategy;

import java.util.Observable;
import java.util.Observer;

public class DataAggregator implements Observer {
	private StepCountStrategy stepCountStrategy;

	public DataAggregator(StepCountStrategy stepCountStrategy) {
		this.stepCountStrategy = stepCountStrategy;
	}

	@Override
	public void update(Observable o, Object dataRecord) {
		System.out.println(this.stepCountStrategy.getTotalSteps());
	}
}
