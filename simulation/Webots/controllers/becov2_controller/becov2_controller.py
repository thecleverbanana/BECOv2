from controller import Robot

robot = Robot()
timestep = int(robot.getBasicTimeStep())


motor_names = ["LEG_LEFT_LEFT_JOINT", "LEG_LEFT_RIGHT_JOINT",
               "LEG_RIGHT_LEFT_JOINT", "LEG_RIGHT_RIGHT_JOINT"]
motors = [robot.getDevice(name) for name in motor_names]

for m in motors:
    m.setVelocity(0.1)  

phase = 0
step_size = -0.02 
last_move_time = robot.getTime()
move_delay = 8  

while robot.step(timestep) != -1:
    now = robot.getTime()
    if now - last_move_time >= move_delay:
        print(f"Executing Phase {phase + 1}")

        if phase == 0:
            motors[0].setPosition(0.0)
            motors[2].setPosition(0.0)

        elif phase == 1:
            motors[0].setPosition(step_size)
            motors[2].setPosition(step_size)

        elif phase == 2:
            motors[1].setPosition(0.0)
            motors[3].setPosition(0.0)

        elif phase == 3:
            motors[1].setPosition(step_size)
            motors[3].setPosition(step_size)

        phase = (phase + 1) % 4
        last_move_time = now
