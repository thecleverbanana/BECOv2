import numpy as np
from sympy import N
from sympy import im
from sympy import symbols, Eq, solve
from scipy.optimize import fsolve

def solve_B_numeric(xA, yA, xP, yP, H2):
    def equations(p):
        xB, yB = p
        eq1 = (xB - xA)*(xP - xB) + (yB - yA)*(yP - yB)
        eq2 = (xP - xB)**2 + (yP - yB)**2 - H2**2
        return (eq1, eq2)

    # Try two guesses: above and below P
    guesses = [
        ((xP + xA) / 2, yP + abs(H2)),  # likely correct
        ((xP + xA) / 2, yP - abs(H2))   # mirror guess
    ]

    for guess in guesses:
        xB_val, yB_val = fsolve(equations, guess)
        # check physical validity
        if yB_val > yP:
            return xB_val, yB_val

    # fallback if neither valid
    raise ValueError("No valid B found (yB > yP)")



def compute_com(xA_val, yA_val, xP_val, yP_val, L_spine_val, H2_val,
                body_height=1.0, relative_x=0.0, relative_y=0.75):
    """
    Compute the global position of the center of mass (COM) for a rectangular robot.

    Parameters:
        xA, yA: Coordinates of point A (left contact point)
        xP, yP: Coordinates of point P (right spine endpoint)
        L_spine: Length of the robot body (spine length from A to P)
        beta: Pitch angle (in radians), upward rotation around spine
        H2: Vertical leg length at point P
        body_height: Height of the rectangular body
        relative_x: COM x-position in body frame (0 = center, -0.5 = left end)
        relative_y: COM y-position in body frame (0 = bottom, 1 = top)

    Returns:
        xCOM, yCOM: Global coordinates of the COM
    """
    # --- use fast numeric method to solve for B ---
    xB_val, yB_val = solve_B_numeric(xA_val, yA_val, xP_val, yP_val, H2_val)

    # vector AB
    dx = xB_val - xA_val
    dy = yB_val - yA_val
    L = np.hypot(dx, dy)
    ux = dx / L
    uy = dy / L

    # A + 1/2 AB
    x_mid = xA_val + 0.5 * dx
    y_mid = yA_val + 0.5 * dy

    # orthogogal
    up_x = -uy
    up_y = ux

    y_offset = relative_y * body_height

    xCOM = x_mid + y_offset * up_x
    yCOM = y_mid + y_offset * up_y

    return xCOM, yCOM, xA_val, yA_val, xB_val, yB_val, xP_val, yP_val
