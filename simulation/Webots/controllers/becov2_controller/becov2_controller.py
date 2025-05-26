from controller import Robot, Keyboard

robot = Robot()
timestep = int(robot.getBasicTimeStep())

keyboard = Keyboard()
keyboard.enable(timestep)

# Devices
motor_names = [
    "LEG_LEFT_LEFT_JOINT", "LEG_LEFT_RIGHT_JOINT",
    "LEG_RIGHT_LEFT_JOINT", "LEG_RIGHT_RIGHT_JOINT"
]
motors = [robot.getDevice(name) for name in motor_names]
shaft_motor = robot.getDevice("SHAFT_JOINT")
shaft_sensor = robot.getDevice("SHAFT_JOINT_sensor")
shaft_sensor.enable(timestep)

# Parameters
for m in motors:
    m.setVelocity(0.01)
shaft_motor.setVelocity(0.01)

# Shaft target
shaft_target = 0.0

# Movement helpers
def set_leg_positions(ll, lr, rl, rr):
    motors[0].setPosition(ll)
    motors[1].setPosition(lr)
    motors[2].setPosition(rl)
    motors[3].setPosition(rr)

def execute_phase(phase):
    global shaft_target
    if phase == 0:
        print("Phase 0: Reset all joints")
        set_leg_positions(0.0, 0.0, 0.0, 0.0)
        shaft_target = 0.0

    elif phase == 1:
        print("Phase 1: Left legs extended")
        set_leg_positions(-0.02, -0.02, 0.0, 0.0)

    elif phase == 2:
        print("Phase 2: Extend shaft to 0.09")
        shaft_target = 0.09

    elif phase == 3:
        print("Phase 3: Right legs extended")
        set_leg_positions(0.0, 0.0, -0.02, -0.02)

    elif phase == 4:
        print("Phase 4: Retract shaft to 0.0")
        shaft_target = 0.0

# Main loop
while robot.step(timestep) != -1:
    key = keyboard.getKey()
    if key in (ord('0'), ord('1'), ord('2'), ord('3'), ord('4')):
        execute_phase(int(chr(key)))

    # Continuous control: gradually push shaft toward target
    shaft_pos = shaft_sensor.getValue()
    error = shaft_target - shaft_pos

    # If error is significant, update position
    if abs(error) > 1e-4:
        shaft_motor.setPosition(shaft_target)

    print(f"[SHAFT] Current: {shaft_pos:.4f}, Target: {shaft_target:.4f}")
