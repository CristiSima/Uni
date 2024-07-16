#!/bin/bash


docker build -t sprc3_mqtt_adaptor mqtt_adaptor
docker build -t sprc3_mqtt_server  mqtt_server

docker stack deploy -c stack.yml sprc3