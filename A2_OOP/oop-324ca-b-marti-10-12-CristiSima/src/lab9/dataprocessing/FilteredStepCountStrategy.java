package lab9.dataprocessing;

import lab9.storage.*;
import java.util.concurrent.TimeUnit;

public class FilteredStepCountStrategy implements StepCountStrategy {
	private static final int MAX_DIFF_STEPS_CONSECUTIVE_RECORDS = 1000;
	private static final long TIME_FOR_MAX_DIFF = TimeUnit.SECONDS.toMillis(1);

	private DataRepository dataRepository;

	public FilteredStepCountStrategy(DataRepository dataRepository) {
		this.dataRepository = dataRepository;
	}

	public int getTotalSteps() {
		int sum = 0;
		for (SensorData sensor:this.dataRepository.getDatabase()) {
			long nowTimestamp = System.currentTimeMillis();
			if (sensor.getStepsCount() < FilteredStepCountStrategy.MAX_DIFF_STEPS_CONSECUTIVE_RECORDS
    			 && 0 < sensor.getStepsCount()
    			 && nowTimestamp - sensor.getTimestamp() < FilteredStepCountStrategy.TIME_FOR_MAX_DIFF) {
				sum += sensor.getStepsCount();
			}
		}
		return sum;
	}

	public String getStrategyDescription() {
		return "FILTERED";
	}
}
