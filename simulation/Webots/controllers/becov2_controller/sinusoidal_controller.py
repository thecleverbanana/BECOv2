# -*- coding: utf-8 -*-
"""
Webots controller that reproduces the sinusoidal trajectory logic from the original
Arduino sketch. Press the **S** key in the 3‑D window to start/stop the oscillator.

Robot requirements (names can be adapted):
  • Four leg joints named            : LEG_LEFT_LEFT_JOINT, LEG_LEFT_RIGHT_JOINT,
                                       LEG_RIGHT_LEFT_JOINT, LEG_RIGHT_RIGHT_JOINT
  • Prismatic joint for the spine   : SHAFT_JOINT (position sensor optional)
  • InertialUnit (gyro integration) : inertial_unit (optional)

Unit mapping hints (adjust as needed):
  • Leg servos: original 0–1800 (0.1° units)  → radians via (val/10)·π/180
  • Spine PWM: 107.1–251.9 (µs)               → linear 0–0.10 m (example)

The update interval is 5 ms (200 Hz) to match the Arduino loop.
"""

from controller import Robot, Keyboard, InertialUnit
import math

# -----------------------------------------------------------------------------
# Initialisation
# -----------------------------------------------------------------------------
robot = Robot()
timestep = int(robot.getBasicTimeStep())

keyboard = Keyboard()
keyboard.enable(timestep)

# ----- Devices ---------------------------------------------------------------
MOTOR_NAMES = [
    "LEG_LEFT_LEFT_JOINT",  # 0
    "LEG_LEFT_RIGHT_JOINT", # 1
    "LEG_RIGHT_LEFT_JOINT", # 2
    "LEG_RIGHT_RIGHT_JOINT" # 3
]

motors = [robot.getDevice(name) for name in MOTOR_NAMES]
for m in motors:
    m.setVelocity(4.0)          # rad s‑1 (tune to your actuator limits)

spine_motor = robot.getDevice("SHAFT_JOINT")
spine_motor.setVelocity(0.02)   # m s‑1 or rad s‑1 depending on joint type

try:
    imu = robot.getDevice("inertial_unit")
    imu.enable(timestep)
except BaseException:
    imu = None  # optional

# -----------------------------------------------------------------------------
# Trajectory pre‑computation (same maths as Arduino version)
# -----------------------------------------------------------------------------
SPINE_MIN_PWM = 107.1
SPINE_MAX_PWM = 251.9
LEG_MIN = 0
LEG_MAX = 1800

PERIOD = 15.0  # seconds
OMEGA = 2 * math.pi / PERIOD
N_SAMPLES = 300

# Lookup tables ---------------------------------------------------------------
spine_traj = []        # PWM like original sketch (will be mapped below)
leg0_traj = []         # left legs (deg*10)
leg1_traj = []         # right legs (deg*10)
for i in range(N_SAMPLES):
    t = i / N_SAMPLES * PERIOD
    # Spine
    pwm = ((SPINE_MAX_PWM - SPINE_MIN_PWM) / 2.0) * math.cos(OMEGA * t) \
        + ((SPINE_MAX_PWM + SPINE_MIN_PWM) / 2.0)
    spine_traj.append(pwm)
    # Legs (theta0/1 as in original)
    theta0 = math.acos(math.cos(OMEGA * t))
    theta1 = math.acos(math.cos(OMEGA * t + math.pi))
    scale = (LEG_MAX - LEG_MIN) / math.pi
    leg0_traj.append(theta0 * scale)
    leg1_traj.append(theta1 * scale)

del i, t, theta0, theta1, scale, pwm  # clean namespace

# Conversion helpers ----------------------------------------------------------
def deg10_to_rad(val: float) -> float:
    """Convert value in 0.1° to radians."""
    return (val / 10.0) * math.pi / 180.0


def pwm_to_spine_pos(pwm: float) -> float:
    """Map PWM range to a 0–0.10 m linear stroke (edit to match your model)."""
    return (pwm - SPINE_MIN_PWM) / (SPINE_MAX_PWM - SPINE_MIN_PWM) * 0.10


# Actuation wrappers ----------------------------------------------------------

def set_leg_positions(l0: float, l1: float):
    """Apply identical angles to left pair and right pair."""
    rad_l0 = deg10_to_rad(l0)
    rad_l1 = deg10_to_rad(l1)
    # Left pair (index 0,1) ← theta0 ; Right pair (2,3) ← theta1
    motors[0].setPosition(rad_l0)
    motors[1].setPosition(rad_l0)
    motors[2].setPosition(rad_l1)
    motors[3].setPosition(rad_l1)


def set_spine_position(pwm: float):
    spine_motor.setPosition(pwm_to_spine_pos(pwm))


# -----------------------------------------------------------------------------
# Control loop
# -----------------------------------------------------------------------------
UPDATE_INTERVAL = 0.005  # seconds (5 ms)
last_update = robot.getTime()

running = False
start_time = 0.0

print("[PyController] Ready – press 'S' to start/stop.")

while robot.step(timestep) != -1:
    # ----- Keyboard toggle ----------------------------------------------------
    key = keyboard.getKey()
    if key in (ord('S'), ord('s')):
        running = not running
        if running:
            start_time = robot.getTime()
            print("[PyController] Oscillator started.")
        else:
            print("[PyController] Oscillator stopped.")

    if not running:
        continue

    now = robot.getTime()
    if now - last_update < UPDATE_INTERVAL:
        continue  # keep update rate ~200 Hz
    last_update = now

    # ----- Index in pre‑computed trajectory ----------------------------------
    elapsed = now - start_time
    phase_time = elapsed % PERIOD
    idx = int(phase_time / PERIOD * N_SAMPLES)

    # Safety clamp (should not be needed)
    idx = max(0, min(N_SAMPLES - 1, idx))

    # ----- Apply trajectory ---------------------------------------------------
    l0 = leg0_traj[idx]
    l1 = leg1_traj[idx]
    pwm = spine_traj[idx]

    set_leg_positions(l0, l1)
    set_spine_position(pwm)

    # ----- Optional IMU integration (gyro‑only yaw estimate) -----------------
    if imu is not None:
        # getRollPitchYaw() returns **angles**, not angular rate; gyro is simpler:
        gyr = imu.getAngularVelocity()  # rad s‑1 vector
        beta_acc = gyr[1] * 180.0 / math.pi  # convert to °/s (Y‑axis)
    else:
        beta_acc = 0.0

    # Log a concise CSV line (leg0, leg1, spine, beta_acc)
    print(f"{l0:.1f},{l1:.1f},{pwm:.1f},{beta_acc:.2f}")
