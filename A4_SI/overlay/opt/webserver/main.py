#!/usr/bin/env python3
from flask import Flask, render_template
import json
import os

os.chdir("/opt/webserver/")
app = Flask(__name__)

@app.context_processor
def renderers():
    return {
        "list": list,
        "type": type,
    }

def get_sensor_data():
    with open("/var/run/sensor_data", "r") as f:
        print(f.read(), flush=True)
    with open("/var/run/sensor_data", "r") as f:
        return json.load(f)


@app.get("/")
def main_page():
    return render_template("sensor_data.html.j2",
        sensor_data=get_sensor_data(),
        render_template=render_template
    )

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=80, debug=True)