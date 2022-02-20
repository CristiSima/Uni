package lab9.communication;

import lab9.storage.*;
import lab9.main.Utils;

import java.util.Observable;
import java.util.Observer;

public class ServerCommunicationController implements Observer {
	@Override
	public void update(Observable o, Object dataRecord) {
		ServerMessage serverMessage = new ServerMessage(((SensorData)dataRecord).getStepsCount(),
                                                		  Utils.getClientId(),
                                                		  System.currentTimeMillis());
		System.out.println(serverMessage);
	}
}
