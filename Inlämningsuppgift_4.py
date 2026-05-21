# MM5016
# Fabian Lleshi - Inlämningsuppgift (Day 13)

import numpy as np
import matplotlib.pyplot as plt
from math import pi


# Exercise - CG for ODE-BVP


def assemble_A(N):
    h = (pi/2) / (N+1)
    A = np.zeros((N, N))
    A[0, 0] = -2/h**2 + 1
    A[0, 1] = 1/h**2
    for i in range(1, N-1):
        A[i, i-1] = 1/h**2
        A[i, i]   = -2/h**2 + 1
        A[i, i+1] = 1/h**2
    A[N-1, N-2] = 1/h**2
    A[N-1, N-1] = -2/h**2 + 1
    return A

def assemble_F(N, leftbc, rightbc):
    h = (pi/2) / (N+1)
    F = np.zeros(N)
    F[0]  = -(1/h**2) * leftbc
    F[-1] = -(1/h**2) * rightbc
    return F

# 1) since the matrix is not SPD, we have to multiply both sides by -1 to make it SPD

# 2) CG implementation

def cg(A, b, x_cg, maxit):
    rk = b - np.dot(A, x_cg)
    p = rk
    for k in np.arange(maxit):
        alpha = np.dot(rk, rk) / np.dot(p, np.dot(A, p))
        x_cg = x_cg + alpha * p
        rkp1 = rk - alpha * np.dot(A, p)
        beta = np.dot(rkp1, rkp1) / np.dot(rk, rk)
        p = rkp1 + beta * p
        rk = rkp1
    return x_cg

# 3) solve BVP with N=100 and rightbc = 2

N = 100
leftbc = 1
rightbc = 2

A = assemble_A(N)
F = assemble_F(N, leftbc, rightbc)


# we have to multiply by -1 to get SPD

A_spd = -A
F_spd = -F

x0 = np.zeros(N)


# we try different iteration counts

x_exact = np.linspace(0, pi/2, N+2)
y_exact = np.cos(x_exact) + 2*np.sin(x_exact)

for n_iter in [5, 10, 20, 50, 100]:
    y_cg = cg(A_spd, F_spd, np.zeros(N), n_iter)
    y_full = np.zeros(N+2)
    y_full[0] = leftbc
    y_full[1:N+1] = y_cg
    y_full[-1] = rightbc
    err = np.max(np.abs(y_full - y_exact))
    print(f'CG {n_iter} iter: max error = {err:.2e}')

# plot for the good one

y_cg = cg(A_spd, F_spd, np.zeros(N), 100)
y_full = np.zeros(N+2)
y_full[0] = leftbc
y_full[1:N+1] = y_cg
y_full[-1] = rightbc

x_fine = np.linspace(0, pi/2, 500)
y_fine = np.cos(x_fine) + 2*np.sin(x_fine)

plt.figure()
plt.plot(x_fine, y_fine, 'k-', label='analytical')
plt.plot(x_exact, y_full, 'b--o', markersize=3, label='CG')
plt.xlabel('x'); plt.ylabel('y(x)')
plt.title(f'CG method, N={N}')
plt.legend(); plt.grid(); plt.show()

# 3) we would need about 100 itearations for N = 100 to match the analytical solution.
# the condition number is around 5500 so convergence is slow without preconditioning.

# 4) CG needs about 100 iterations for N = 100.
# Jacobi would need way more for the same N, so CG is much better.