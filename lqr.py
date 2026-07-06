import numpy as np
from scipy.linalg import solve_continuous_are

def get_gain(q_pos=50,
             q_vel=8,
             q_theta=30,
             q_theta_dot=10,
             r=10):

    M = 10.0
    m = 1.0
    l = 1.0
    g = 9.81

    A = np.array([
        [0, 1, 0, 0],
        [0, 0, -(m * g) / M, 0],
        [0, 0, 0, 1],
        [0, 0, (M + m) * g / (M * l), 0]
    ])

    B = np.array([
        [0],
        [1 / M],
        [0],
        [-1 / (M * l)]
    ])

    #Weight matrices
    Q = np.diag([
        q_pos,
        q_vel,
        q_theta,
        q_theta_dot
    ])

    R = np.array([[r]])

    P = solve_continuous_are(A, B, Q, R)

    K = np.linalg.inv(R) @ B.T @ P

    return K

if __name__ == "__main__":
    K = get_gain(
        q_pos=50,
        q_vel=8,
        q_theta=30,
        q_theta_dot=10,
        r=10
    )

    print("LQR Gain Matrix:")
    print(K)