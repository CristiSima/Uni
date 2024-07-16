#!/usr/bin/env python3
import serial
import os
import json
from functools import partial
from threading import Thread
print = partial(print, flush=True)

state={
    "ext":{
        "humidity": None,
        "temp": None,
        "rain": None,
    },
    "int":{
        "humidity": None,
        "temp": None,
    }
}

def update_state_from_serial(dev_path, out_path):
    ser = serial.Serial(dev_path)  # open serial port
    out_file = open(out_path, "w")

    json.dump(state, out_file)
    out_file.flush()

    while True:
        try:
            sensor_data = ser.readline()
            if sensor_data[-1] != ord("\n"):
                print("Incomplete data", sensor_data)
                continue

            sensor_data = list(map(lambda x:x.strip(";"), sensor_data.strip().decode().split(" ")))
        except Exception as e:
            print(e)
            continue

        if "ERROR" in sensor_data or len(sensor_data) != 4:
            print("Sensor had error ", sensor_data)
            continue

        if sensor_data[1] == "ext":
            if sensor_data[2] not in {"temp", "humidity", "rain"}:
                print("Exterior sensor outside of expected fields:", sensor_data[2])
                continue
        elif sensor_data[1] == "int":
            if sensor_data[2] not in {"temp", "humidity"}:
                print("Interior sensor outside of expected fields:", sensor_data[2])
                continue
        else:
            print("Sensor data location:", sensor_data[1])
            continue
        
        try:
            value = float(sensor_data[3])
        except Exception as e:
            print("Value is not numeric", sensor_data[3])
            continue

        if sensor_data[2] == "rain":
            state[sensor_data[1]][sensor_data[2]] = sensor_data[0], value > 0
        else:
            state[sensor_data[1]][sensor_data[2]] = sensor_data[0], value

        # write to file
        out_file.seek(0)
        out_file.truncate()
        json.dump(state, out_file)
        out_file.flush()
    ser.close() 


if __name__ == "__main__":
    update_state_from_serial('/dev/ttyUSB0', '/var/run/sensor_data')
    # state_update_thread = Thread(target=update_state_from_serial, args=('/dev/ttyUSB0',))
    # state_serve_thread = Thread(target=serve_state_to_socket, args=('/tmp/serve',))


    # state_update_thread.run()
    
