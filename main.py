import numpy as np
import control

M = 1.0      
m = 0.2      
l = 0.5      
g = 9.81

A = np.array([
    [0, 1, 0, 0],
    [0, 0, -(m*g)/M, 0],
    [0, 0, 0, 1],
    [0, 0, ((M+m)*g)/(M*l), 0]
])

B = np.array([
    [0],
    [1/M],
    [0],
    [-1/(M*l)]
])

Q = np.diag([
    10,     
    1,      
    100,    
    10      
])

R = np.array([[0.1]])

K, S, E = control.lqr(A, B, Q, R)

print("Gain matrix K:")
print(K)

print("\nClosed-loop poles:")
print(E)