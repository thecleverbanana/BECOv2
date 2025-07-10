from controller import Robot, Keyboard
import math

robot = Robot()
timestep = int(robot.getBasicTimeStep())

keyboard = Keyboard()
keyboard.enable(timestep)

# -----------------------------------------------------------------------------
# Leg motors only (pure position control, NO setVelocity) ----------------------
# -----------------------------------------------------------------------------
MOTOR_NAMES = [
    "LEG_LEFT_LEFT_JOINT", "LEG_LEFT_RIGHT_JOINT",
    "LEG_RIGHT_LEFT_JOINT", "LEG_RIGHT_RIGHT_JOINT"
]

motors = [robot.getDevice(n) for n in MOTOR_NAMES]

# -----------------------------------------------------------------------------
# Trajectory: simple cosine swing 0 … −0.02 rad (left) / 0.02 rad (right) -----
# -----------------------------------------------------------------------------
PERIOD = 15.0  # seconds for one full cycle
OMEGA = 2 * math.pi / PERIOD
N      = 300   # lookup table resolution

leg_traj = [-0.02 * math.cos(OMEGA * t * PERIOD / N) for t in range(N)]

# -----------------------------------------------------------------------------
# Runtime toggling -------------------------------------------------------------
# -----------------------------------------------------------------------------
running      = False
start_time   = 0.0
last_update  = robot.getTime()
UPDATE_DT    = 0.005  # 200 Hz
print("[Leg-only] Press 'S' to start/stop")

while robot.step(timestep) != -1:
    if keyboard.getKey() in (ord('S'), ord('s')):
        running = not running
        start_time = robot.getTime()
        print("Running..." if running else "Stopped.")

    if not running:
        continue

    now = robot.getTime()
    if now - last_update < UPDATE_DT:
        continue
    last_update = now

    phase = (now - start_time) % PERIOD
    idx = int(phase / PERIOD * N)
    angle = leg_traj[idx]

    # First two motors (left side) follow angle, right side mirror
    for i, m in enumerate(motors):
        m.setPosition(angle if i < 2 else -angle)