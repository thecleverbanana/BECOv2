import numpy as np

def build_static_matrix(t1, t2, n1, n2, xA, yA, xB, yB, xCOM, mass, g=9.81):
    """
    Construct the static equilibrium linear system:
    A @ [f1, f2, N_A, N_B] = b

    Parameters:
        t1, t2: Tangent unit vectors [tx, ty] at contact points A and B
        n1, n2: Normal unit vectors [nx, ny] at contact points A and B
        xA, yA: Coordinates of point A
        xB, yB: Coordinates of point B
        xCOM: x-coordinate of the center of mass
        mass: Total mass of the body
        g: Gravitational acceleration (default is 9.81 m/s²)

    Returns:
        A: 4×4 coefficient matrix
        b: Right-hand side vector (4×1)
    """
    # Coordinate differences
    dx_BA = xB - xA
    dy_BA = yB - yA
    dx_AB = -dx_BA
    dy_AB = -dy_BA

    # Coefficient matrix A
    A = np.array([
        [t1[0],      t2[0],      n1[0],      n2[0]],
        [t1[1],      t2[1],      n1[1],      n2[1]],
        [0,          dx_BA * t2[1] - dy_BA * t2[0], 0, dx_BA * n2[1] - dy_BA * n2[0]],
        [dx_AB * t1[1] - dy_AB * t1[0], 0, dx_AB * n1[1] - dy_AB * n1[0], 0]
    ])

    # Right-hand side vector b
    b = np.array([
        0,
        mass * g,
        mass * g * (xCOM - xA),
        mass * g * (xCOM - xB)
    ])

    return A, b
