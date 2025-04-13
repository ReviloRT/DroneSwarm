import serial
import matplotlib.pyplot as plt
import numpy as np
import re
import time

# ---- Config ----
SERIAL_PORT = 'COM3'  # adjust as needed
BAUD_RATE = 115200
READ_TIMEOUT = 0.1

# ---- Init Serial ----
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=READ_TIMEOUT)
ser.reset_input_buffer()



def parse_data(text):
    lines = text.strip().splitlines()
    data = {}
    current_matrix = None
    matrix_rows = []

    for line in lines:
        line = line.strip()

        # Check for matrix start
        if line.startswith("R:["):
            current_matrix = "R"
            matrix_rows = []
            continue
        elif line.startswith("dR:["):
            current_matrix = "dR"
            matrix_rows = []
            continue
        elif line.startswith("[") and line.endswith("]") and current_matrix:
            # Inside a matrix
            row = [float(x.strip()) for x in line.strip("[]").split(",")]
            matrix_rows.append(row)
            continue
        elif current_matrix and line == "]":
            # Matrix finished
            data[current_matrix] = matrix_rows
            current_matrix = None
            matrix_rows = []
            continue

        # Scalar line (e.g., >px:-5.065860)
        if line.startswith(">"):
            try:
                key, val = line[1:].split(":", 1)
                data[key.strip()] = float(val.strip())
            except ValueError:
                continue  # malformed line
    return data

def print_data(data):
    px, py, pz = data.get('px', 0), data.get('py', 0), data.get('pz', 0)
    vx, vy, vz = data.get('vx', 0), data.get('vy', 0), data.get('vz', 0)
    dpx, dpy, dpz = data.get('dpx', 0), data.get('dpy', 0), data.get('dpz', 0)
    dvx, dvy, dvz = data.get('dvx', 0), data.get('dvy', 0), data.get('dvz', 0)
    ax, ay, az = data.get('rax', 0), data.get('ray', 0), data.get('raz', 0)
    gx, gy, gz = data.get('rgx', 0), data.get('rgy', 0), data.get('rgz', 0)
    gax, gay, gaz = data.get('gax', 0), data.get('gay', 0), data.get('gaz', 0)
    t , dt, rt =  data.get('t', 0), data.get('dt', 0), data.get('rt', 0)
    
    print("\n--- Data Vectors ---")
    print(f"Time, Delta, Raw:   { t:9.3f}  {dt:9.5f}  {rt:9.9f}")
    print(f"Position        : [ {px:9.3f}, {py:9.3f}, {pz:9.3f} ]")
    print(f"Velocity        : [ {vx:9.3f}, {vy:9.3f}, {vz:9.3f} ]")
    print(f"Delta Position  : [ {dpx:9.3f}, {dpy:9.3f}, {dpz:9.3f} ]")
    print(f"Delta Velocity  : [ {dvx:9.3f}, {dvy:9.3f}, {dvz:9.3f} ]")
    print(f"Acceleration    : [ {ax:9.5f}, {ay:9.5f}, {az:9.5f} ]")
    print(f"Angular         : [ {gx:9.5f}, {gy:9.5f}, {gz:9.5f} ]")
    print(f"Gravity         : [ {gax:9.5f}, {gay:9.5f}, {gaz:9.5f} ]")


# # ---- Plot Setup ----
plt.ion()

# Setup figure and 3D axis
fig = plt.figure()
ax_pos = fig.add_subplot(111, projection='3d')
ax_pos.set_xlabel('X')
ax_pos.set_ylabel('Y')
ax_pos.set_zlabel('Z')
ax_pos.set_title('Trajectory of Position Vector')

# Store position history
position_history = []

def update_plot(data):
    # Get position, velocity, acceleration
    px, py, pz = data.get('px', 0), data.get('py', 0), data.get('pz', 0)
    vx, vy, vz = data.get('vx', 0), data.get('vy', 0), data.get('vz', 0)
    axx, axy, axz = data.get('ax', 0), data.get('ay', 0), data.get('az', 0)

    # Append current position
    position_history.append((px, py, pz))

    # Limit trail length
    max_length = 500
    if len(position_history) > max_length:
        position_history.pop(0)

    # Clear plot
    ax_pos.clear()
    ax_pos.set_title('Trajectory with Velocity and Acceleration')
    ax_pos.set_xlabel('X')
    ax_pos.set_ylabel('Y')
    ax_pos.set_zlabel('Z')

    # Plot trajectory
    xs, ys, zs = zip(*position_history)
    ax_pos.plot(xs, ys, zs, color='blue', label='Position Trace')
    ax_pos.scatter(px, py, pz, color='red', label='Current Position')

    # Plot velocity vector
    ax_pos.quiver(px, py, pz, vx, vy, vz, color='green', length=1, normalize=True, label='Velocity')

    # Plot acceleration vector
    ax_pos.quiver(px, py, pz, axx, axy, axz, color='orange', length=1, normalize=True, label='Acceleration')

    ax_pos.legend()
    plt.pause(0.001)

# ---- Main Loop ----
while True:
    time.sleep(0.5)
    try:
        if ser.in_waiting:
            buffer = ser.read(ser.in_waiting).decode('utf-8',errors='ignore')
            # Use the most recent timestamp block
            chunks = buffer.split(">t:")
            print(len(chunks))
            if len(chunks) > 2:
                latest_block = ">t:" + chunks[-2]
                data = parse_data(latest_block)
                print_data(data)
                # print(data)
                update_plot(data)
    except KeyboardInterrupt:
        break
    except Exception as e:
        print("Error:", e)

ser.close()
