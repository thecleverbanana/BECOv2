from controller import Supervisor
import csv
import os

supervisor = Supervisor()
timestep = int(supervisor.getBasicTimeStep())

robot_node = supervisor.getFromDef("BECOV2_INSTANCE")
if robot_node is None:
    print("[ERROR] Could not find node with DEF name 'BECOV2_INSTANCE'")
    exit(1)

log_path = os.path.join(os.path.dirname(__file__), "trajectory_log.csv")
with open(log_path, "w", newline="") as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow(["time", "x", "y", "z", "shaft_length"])

marker = None
children_field = supervisor.getRoot().getField("children")
index = children_field.getCount()
children_field.importMFNodeFromString(-1, """
Solid {
  translation 0 0 0
  name "COM_MARKER"
  children [
    Shape {
      appearance PBRAppearance {
        baseColor 1 0 0
      }
      geometry Sphere {
        radius 0.005
      }
    }
  ]
  physics NULL
}
""")
marker = children_field.getMFNode(index)


COM_THRESHOLD = 1e-4 

last_com = None
step_count = 0

while supervisor.step(timestep) != -1:
    com_pos = robot_node.getCenterOfMass()
    time_sec = step_count * timestep / 1000.0

    should_log = False
    if last_com is None:
        should_log = True
    else:
        delta = [(com_pos[i] - last_com[i]) for i in range(3)]
        movement = sum(d**2 for d in delta)**0.5
        if movement > COM_THRESHOLD:
            should_log = True
            
    # Read Shaft State
    try:
        with open("../becov2_controller/shaft_state.txt", "r") as f:
            shaft_value = float(f.read().strip())
    except:
        shaft_value = -1

    if should_log:
        with open(log_path, "a", newline="") as csvfile:
            writer = csv.writer(csvfile)
            writer.writerow([f"{time_sec:.3f}", *["%.6f" % v for v in com_pos], "%.4f" % shaft_value])

        print(f"[COM] t={time_sec:.2f}s -> x={com_pos[0]:.4f}, y={com_pos[1]:.4f}, z={com_pos[2]:.4f}")

        last_com = com_pos
        marker.getField("translation").setSFVec3f(com_pos)

    step_count += 1

