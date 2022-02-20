package lab9.storage;


import java.util.List;
import java.util.ArrayList;
import java.util.Observable;

/**
 * Persists sensor data. Observable, its observers are notified when data is added it to.
 */
public class DataRepository extends Observable { // TODO make this an Observable
	private List<SensorData> database;

	public DataRepository() {
		this.database = new ArrayList();
	}

	public void addData(SensorData dataRecord) {
		// TODO
		// store data (e.g. in a List)
		this.database.add(dataRecord);
		this.setChanged();
		// notifiy observers
		this.notifyObservers(dataRecord);
	}

	// TODO implement a method to get the stored data (needed by the strategies)
	public List<SensorData> getDatabase() {
		return this.database;
	}
}
