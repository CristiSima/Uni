import paho.mqtt.client as mqtt
import json
import time
import logging
from datetime import datetime
from typing import Union
from os import environ
from influxdb_client import InfluxDBClient, WriteApi, WriteOptions, Point, WritePrecision

# wait for DB to initialize
time.sleep(1)

def getLogger():
    logger = logging.getLogger(__name__)
    logger.setLevel(logging.ERROR)
    if environ.get("DEBUG_DATA_FLOW", "") == "true":
        logger.setLevel(logging.INFO)

    formatter = logging.Formatter(
        "%(asctime)s %(message)s", 
        datefmt="%Y-%m-%d %H:%M:%S"
        )

    handler = logging.StreamHandler()
    handler.setFormatter(formatter)

    logger.addHandler(handler)

    return logger

logger=getLogger()


db_client = InfluxDBClient(
    url="http://db:8086", 
    username=environ.get("DOCKER_INFLUXDB_INIT_USERNAME"), 
    password=environ.get("DOCKER_INFLUXDB_INIT_PASSWORD"),
    org=environ.get("DOCKER_INFLUXDB_INIT_ORG"),
    debug=False
)
bucket_api = db_client.buckets_api()
write_api = db_client.write_api(write_options=WriteOptions(batch_size=1))


def on_connect(client, userdata, flags, rc):
    logger.debug("Adapter connected with result code "+str(rc))

    client.subscribe("#")

def on_message(client, userdata, msg):
    if msg.topic.startswith("$SYS/"):
        logger.debug("sys msg")
        return

    logger.info(f"Received a message by topic [{msg.topic}]")
    data = json.loads(msg.payload)

    payload = {
        "measurement": msg.topic.split("/")[1],
        "tags":{
            "location": msg.topic.split("/")[0],
        },
        "fields":{
            key:val for key,val in data.items()
            if isinstance(val, Union[int, float])
        }
    }

    if "timestamp" in data:
        payload["time"] = data["timestamp"]
        logger.info(f"Data timestamp is {data['timestamp']}")
    else:
        logger.info("Data timestamp is NOW")

    for key,val in payload["fields"].items():
        logger.info(f"{msg.topic}.{key} {val}")

    logger.info("")
    logger.debug(msg.topic, data, payload)

    write_api.write("sensor_data", "SPRC_T3", payload)
    

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("mqtt_server", 1883, 60)


client.loop_forever()
