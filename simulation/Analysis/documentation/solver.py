import sympy as sp

f1, f2, N_A, N_B = sp.symbols('f1 f2 N_A N_B')
theta1, theta2 = sp.symbols('theta1 theta2')
X_A, Y_A, X_B, Y_B, X_COM = sp.symbols('X_A Y_A X_B Y_B X_COM')
m, g = sp.symbols('m g')

C1 = sp.cos(theta1)
S1 = sp.sin(theta1)
C2 = sp.cos(theta2)
S2 = sp.sin(theta2)

dx = X_B - X_A
dy = Y_B - Y_A
dxc1 = X_COM - X_A
dxc2 = X_COM - X_B

eq1 = -f1*C1 + f2*C2 - N_A*S1 - N_B*S2
eq2 = f1*S1 + f2*S2 - N_A*C1 + N_B*C2 - m*g
eq3 = dx*(f2*S2 + N_B*C2) - dy*(f2*C2 - N_B*S2) - m*g*dxc1
eq4 = -dx*(f1*S1 - N_A*C1) - dy*(-f1*C1 - N_A*S1) - m*g*dxc2

solution = sp.solve([eq1, eq2, eq3, eq4], (f1, f2, N_A, N_B), dict=True)[0]

for var, expr in solution.items():
    print(f"{var} =")
    sp.pprint(sp.simplify(expr), use_unicode=True)
    print()
