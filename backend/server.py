from flask import Flask, request, jsonify
from flask_cors import CORS

import subprocess
import json
import os

app = Flask(__name__)
CORS(app)

# =========================================================
# Home Route
# =========================================================

@app.route("/")
def home():
    return "ArchTrade Backend Running"


# =========================================================
# Simulator executable path
# =========================================================

BASE_DIR = os.path.dirname(os.path.abspath(__file__))

SIMULATOR_PATH = os.path.join(BASE_DIR, "ArchTrade_web.exe")


# =========================================================
# Helper function to run simulator
# =========================================================

def execute_simulation(config):

    command = [

        SIMULATOR_PATH,

        config["workload"],
        config["instruction_set"],
        config["pipeline"],
        config["cache"],
        config["io_type"],

        str(config["cache_size"]),
        str(config["cache_latency"]),
        str(config["ram_latency"]),
        str(config["memory_bus_latency"]),
        str(config["alu_latency"]),
        str(config["decode_latency"]),
        str(config["device_latency"])
    ]

    print("\n==============================")
    print("Running Command:")
    print(command)
    print("==============================\n")

    result = subprocess.run(
        command,
        capture_output=True,
        text=True
    )

    print("STDOUT:")
    print(result.stdout)

    print("STDERR:")
    print(result.stderr)

    if result.returncode != 0:

        return {
            "success": False,
            "error": result.stderr
        }

    try:

        parsed_output = json.loads(result.stdout)

        return {
            "success": True,
            "data": parsed_output
        }

    except Exception as e:

        return {
            "success": False,
            "error": str(e),
            "raw_output": result.stdout
        }


# =========================================================
# Run single simulation
# =========================================================

@app.route("/run", methods=["POST"])
def run_simulation():

    try:

        data = request.json

        result = execute_simulation(data)

        if not result["success"]:

            return jsonify({
                "error": result["error"],
                "raw_output": result.get("raw_output", "")
            }), 500

        return jsonify(result["data"])

    except Exception as e:

        return jsonify({
            "error": str(e)
        }), 500


# =========================================================
# Compare two simulations
# =========================================================

@app.route("/compare", methods=["POST"])
def compare_simulations():

    try:

        data = request.json

        config1 = data["config1"]
        config2 = data["config2"]

        result1 = execute_simulation(config1)
        result2 = execute_simulation(config2)

        return jsonify({
            "config1": result1,
            "config2": result2
        })

    except Exception as e:

        return jsonify({
            "error": str(e)
        }), 500


# =========================================================
# Start Flask server
# =========================================================

if __name__ == "__main__":

    app.run(
        host="0.0.0.0",
        port=5000,
        debug=True
    )