from flask import Flask, request, jsonify

app = Flask(__name__)

# Motor state
motor_state = {"direction": "stop", "speed": 150}  # Default speed

@app.route('/')
def index():
    return '''
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Motor Control Dashboard</title>
        <script src="https://cdn.tailwindcss.com"></script>
        <script src="https://cdn.jsdelivr.net/npm/axios/dist/axios.min.js"></script>
    </head>
    <body class="flex flex-col items-center justify-center min-h-screen bg-gray-100">
        <h1 class="text-4xl font-bold mb-4">Motor Control</h1>

        <div class="flex gap-4">
            <button onclick="controlMotor('forward')" class="px-6 py-2 bg-green-500 text-white rounded">Forward</button>
            <button onclick="controlMotor('stop')" class="px-6 py-2 bg-gray-500 text-white rounded">Stop</button>
            <button onclick="controlMotor('reverse')" class="px-6 py-2 bg-red-500 text-white rounded">Reverse</button>
        </div>

        <div class="mt-4">
            <label for="speed" class="block text-lg">Speed:</label>
            <input type="range" id="speed" min="0" max="255" value="150" class="w-64" oninput="updateSpeed(this.value)">
            <span id="speed-value">150</span>
        </div>

        <script>
            function controlMotor(direction) {
                axios.post(`/motor/${direction}`).catch(err => console.error(err));
            }

            function updateSpeed(value) {
                document.getElementById("speed-value").textContent = value;
                axios.post(`/motor/speed?value=${value}`).catch(err => console.error(err));
            }
        </script>
    </body>
    </html>
    '''

# --- MOTOR CONTROL API ENDPOINTS ---
@app.route('/motor', methods=['GET'])
def get_motor_state():
    return jsonify(motor_state)

@app.route('/motor/forward', methods=['POST'])
def motor_forward():
    motor_state["direction"] = "forward"
    return jsonify(motor_state)

@app.route('/motor/reverse', methods=['POST'])
def motor_reverse():
    motor_state["direction"] = "reverse"
    return jsonify(motor_state)

@app.route('/motor/stop', methods=['POST'])
def motor_stop():
    motor_state["direction"] = "stop"
    return jsonify(motor_state)

@app.route('/motor/speed', methods=['POST'])
def motor_speed():
    speed = request.args.get("value", type=int)
    if speed is not None:
        motor_state["speed"] = speed
    return jsonify(motor_state)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
