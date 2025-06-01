from controller import Robot

robot = Robot()
timestep = int(robot.getBasicTimeStep())

# Motors & sensors
motor_names = [
    "LEG_LEFT_LEFT_JOINT", "LEG_LEFT_RIGHT_JOINT",
    "LEG_RIGHT_LEFT_JOINT", "LEG_RIGHT_RIGHT_JOINT"
]
motors = [robot.getDevice(name) for name in motor_names]
shaft_motor = robot.getDevice("SHAFT_JOINT")
shaft_sensor = robot.getDevice("SHAFT_JOINT_sensor")
shaft_sensor.enable(timestep)

# Velocity setting
for m in motors:
    m.setVelocity(0.01)
shaft_motor.setVelocity(0.01)

# Movement helpers
def set_leg_positions(ll, lr, rl, rr):
    motors[0].setPosition(ll)
    motors[1].setPosition(lr)
    motors[2].setPosition(rl)
    motors[3].setPosition(rr)

def wait_until_shaft_reaches(target, tolerance=1e-4):
    while robot.step(timestep) != -1:
        pos = shaft_sensor.getValue()
        error = abs(pos - target)
        if error < tolerance:
            break
        shaft_motor.setPosition(target)

def pause(duration_ms):
    steps = int(duration_ms / timestep)
    for _ in range(steps):
        robot.step(timestep)

# Action sequence
def run_sequence(shaft_val):
    print(f"=== Starting sequence for shaft_target = {shaft_val:.2f} ===")

    set_leg_positions(0.0, 0.0, 0.0, 0.0)
    pause(10000)

    set_leg_positions(-0.02, -0.02, 0.0, 0.0)
    pause(10000)

    set_leg_positions(0.0, 0.0, 0.0, 0.0)
    pause(10000)

    set_leg_positions(0.0, 0.0, -0.02, -0.02)
    pause(10000)

    set_leg_positions(0.0, 0.0, 0.0, 0.0)
    pause(10000)

    set_leg_positions(-0.02, -0.02, 0.0, 0.0)
    pause(10000)

    set_leg_positions(0.0, 0.0, -0.02, -0.02)
    pause(10000)

    set_leg_positions(-0.02, -0.02, 0.0, 0.0)
    pause(10000)

    set_leg_positions(0.0, 0.0, -0.02, -0.02)
    pause(10000)

    set_leg_positions(0.0, 0.0, 0.0, 0.0)
    pause(10000)

    # Set shaft
    print(f"[SHAFT] Moving to {shaft_val:.2f}")
    shaft_motor.setPosition(shaft_val)
    wait_until_shaft_reaches(shaft_val)
    pause(10000)

# === Main loop ===
shaft_vals = [round(i * 0.01, 2) for i in range(10)]  # 0.00 to 0.09

for val in shaft_vals:
    run_sequence(val)

print("All sequences completed.")
