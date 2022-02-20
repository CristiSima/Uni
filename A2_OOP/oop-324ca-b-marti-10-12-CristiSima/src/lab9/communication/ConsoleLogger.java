package lab9.communication;

import java.util.Observer;
import java.util.Observable;

public class ConsoleLogger implements Observer {
	@Override
	public void update(Observable o, Object dataRecord) {
		System.out.println("new SensorData added");
	}
}
