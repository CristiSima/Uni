package lab9.dataprocessing;

import lab9.storage.*;

public class BasicStepCountStrategy implements StepCountStrategy {
	private DataRepository dataRepository;

	public BasicStepCountStrategy(DataRepository dataRepository) {
		this.dataRepository = dataRepository;
	}

	public int getTotalSteps() {
		int sum = 0;
		for (SensorData sensor:this.dataRepository.getDatabase()) {
			sum += sensor.getStepsCount();
		}
		return sum;
	}

	public String getStrategyDescription() {
		return "BASIC";
	}
}
