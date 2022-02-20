package lab9.dataprocessing;

import lab9.main.Utils;
import lab9.storage.DataRepository;

public class StepCountStrategyFactory {
	public StepCountStrategy createStrategy(String strategyType, DataRepository dataRepository) {
		switch (strategyType) {
		case Utils.BASIC_STRATEGY:
			return new BasicStepCountStrategy(dataRepository);
		case Utils.FILTERED_STRATEGY:
			return new FilteredStepCountStrategy(dataRepository);
		default:
			return null;
		}
	}

}
