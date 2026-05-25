from flask import Flask, render_template, request, jsonify
import subprocess
import os
import re

app = Flask(__name__, template_folder="templates", static_folder="static")


def parse_simulator_output(output):
    """Parse the C++ simulator stdout into a structured dict."""
    result = {
        "cycles":            None,
        "cpi":               None,
        "cacheHits":         None,
        "cacheMisses":       None,
        "pipelineStalls":    None,
        "cpuIdleCycles":     None,
        "memoryStallCycles": None,
        "branchStallCycles": None,
        "ioStallCycles":     None,
        "executionCycles":   None,
        "raw": output
    }

    patterns = {
        "cycles":            r"(?i)(?:total\s+)?cycles?\s*[:\-=]\s*([\d.]+)",
        "cpi":               r"(?i)cpi\s*[:\-=]\s*([\d.]+)",
        "cacheHits":         r"(?i)cache\s+hits?\s*[:\-=]\s*([\d.]+)",
        "cacheMisses":       r"(?i)cache\s+misses?\s*[:\-=]\s*([\d.]+)",
        "pipelineStalls":    r"(?i)pipeline\s+stalls?\s*[:\-=]\s*([\d.]+)",
        "cpuIdleCycles":     r"(?i)(?:cpu\s+)?idle\s+cycles?\s*[:\-=]\s*([\d.]+)",
        "memoryStallCycles": r"(?i)memory\s+stall\s+cycles?\s*[:\-=]\s*([\d.]+)",
        "branchStallCycles": r"(?i)branch\s+stall\s+cycles?\s*[:\-=]\s*([\d.]+)",
        "ioStallCycles":     r"(?i)io\s+stall\s+cycles?\s*[:\-=]\s*([\d.]+)",
        "executionCycles":   r"(?i)execution\s+cycles?\s*[:\-=]\s*([\d.]+)",
    }

    for key, pattern in patterns.items():
        m = re.search(pattern, output)
        if m:
            val = m.group(1)
            result[key] = float(val) if "." in val else int(val)

    return result


def _run_sim(workload, instruction_set, pipeline, cache, io_type,
             cache_size=32, cache_latency=2, ram_latency=20, mem_bus_latency=1,
             alu_latency=1, decode_latency=1, device_latency=15):
    exe_path = os.path.join(os.getcwd(), "ArchTrade_web.exe")
    process = subprocess.run(
        [
            exe_path,
            workload,
            instruction_set,
            pipeline,
            cache,
            io_type,
            str(cache_size),
            str(cache_latency),
            str(ram_latency),
            str(mem_bus_latency),
            str(alu_latency),
            str(decode_latency),
            str(device_latency),
        ],
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

        workload        = data.get("workload")
        instruction_set = data.get("instruction_set")
        pipeline        = data.get("pipeline")
        cache           = data.get("cache")
        io_type         = data.get("io_type")

        # Optional hardware params (single-run uses defaults)
        cache_size      = data.get("cache_size",      32)
        cache_latency   = data.get("cache_latency",   2)
        ram_latency     = data.get("ram_latency",     20)
        mem_bus_latency = data.get("mem_bus_latency", 1)
        alu_latency     = data.get("alu_latency",     1)
        decode_latency  = data.get("decode_latency",  1)
        device_latency  = data.get("device_latency",  15)

        stdout, _ = _run_sim(
            workload, instruction_set, pipeline, cache, io_type,
            cache_size, cache_latency, ram_latency, mem_bus_latency,
            alu_latency, decode_latency, device_latency
        )

        return jsonify({"ok": True, "output": stdout})

    except Exception as e:
        return jsonify({"ok": False, "error": str(e)})


@app.route("/compare", methods=["POST"])
def compare():
    try:
        data = request.get_json()

        workload = data.get("workload")
        configA  = data.get("configA", {})
        configB  = data.get("configB", {})

        def hw(cfg, key, default):
            return cfg.get(key, default)

        outA, _ = _run_sim(
            workload,
            configA.get("instruction_set"),
            configA.get("pipeline"),
            configA.get("cache"),
            configA.get("io_type"),
            hw(configA, "cache_size",      32),
            hw(configA, "cache_latency",   2),
            hw(configA, "ram_latency",     20),
            hw(configA, "mem_bus_latency", 1),
            hw(configA, "alu_latency",     1),
            hw(configA, "decode_latency",  1),
            hw(configA, "device_latency",  15),
        )

        outB, _ = _run_sim(
            workload,
            configB.get("instruction_set"),
            configB.get("pipeline"),
            configB.get("cache"),
            configB.get("io_type"),
            hw(configB, "cache_size",      32),
            hw(configB, "cache_latency",   2),
            hw(configB, "ram_latency",     20),
            hw(configB, "mem_bus_latency", 1),
            hw(configB, "alu_latency",     1),
            hw(configB, "decode_latency",  1),
            hw(configB, "device_latency",  15),
        )

        parsedA = parse_simulator_output(outA)
        parsedB = parse_simulator_output(outB)

        return jsonify({"ok": True, "A": parsedA, "B": parsedB})

    except Exception as e:
        return jsonify({"ok": False, "error": str(e)})


if __name__ == "__main__":
    app.run(debug=True)