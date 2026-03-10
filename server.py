from flask import Flask, render_template, request, jsonify
import subprocess
import os
import re

app = Flask(__name__, template_folder="templates", static_folder="static")


def parse_simulator_output(output):
    """Parse the C++ simulator stdout into a structured dict."""
    result = {
        "cycles": None,
        "cpi": None,
        "cacheHits": None,
        "cacheMisses": None,
        "cpuIdleCycles": None,
        "raw": output
    }

    patterns = {
        "cycles":       r"(?i)(?:total\s+)?cycles?\s*[:\-=]\s*([\d.]+)",
        "cpi":          r"(?i)cpi\s*[:\-=]\s*([\d.]+)",
        "cacheHits":    r"(?i)cache\s+hits?\s*[:\-=]\s*([\d.]+)",
        "cacheMisses":  r"(?i)cache\s+misses?\s*[:\-=]\s*([\d.]+)",
        "cpuIdleCycles":r"(?i)(?:cpu\s+)?idle\s+cycles?\s*[:\-=]\s*([\d.]+)",
    }

    for key, pattern in patterns.items():
        m = re.search(pattern, output)
        if m:
            val = m.group(1)
            result[key] = float(val) if "." in val else int(val)

    return result


def _run_sim(workload, instruction_set, pipeline, cache, io_type):
    exe_path = os.path.join(os.getcwd(), "ArchTrade_web.exe")
    process = subprocess.run(
        [exe_path, workload, instruction_set, pipeline, cache, io_type],
        capture_output=True,
        text=True
    )
    return process.stdout, process.returncode


@app.route("/")
def index():
    return render_template("index.html")


@app.route("/run", methods=["POST"])
def run_simulation():
    try:
        data = request.get_json()

        workload       = data.get("workload")
        instruction_set = data.get("instruction_set")
        pipeline       = data.get("pipeline")
        cache          = data.get("cache")
        io_type        = data.get("io_type")

        stdout, _ = _run_sim(workload, instruction_set, pipeline, cache, io_type)

        return jsonify({
            "ok": True,
            "output": stdout
        })

    except Exception as e:
        return jsonify({
            "ok": False,
            "error": str(e)
        })


@app.route("/compare", methods=["POST"])
def compare():
    try:
        data = request.get_json()

        workload = data.get("workload")
        configA  = data.get("configA", {})
        configB  = data.get("configB", {})

        outA, _ = _run_sim(
            workload,
            configA.get("instruction_set"),
            configA.get("pipeline"),
            configA.get("cache"),
            configA.get("io_type")
        )

        outB, _ = _run_sim(
            workload,
            configB.get("instruction_set"),
            configB.get("pipeline"),
            configB.get("cache"),
            configB.get("io_type")
        )

        parsedA = parse_simulator_output(outA)
        parsedB = parse_simulator_output(outB)

        return jsonify({
            "ok": True,
            "A": parsedA,
            "B": parsedB
        })

    except Exception as e:
        return jsonify({
            "ok": False,
            "error": str(e)
        })


if __name__ == "__main__":
    app.run(debug=True)