import numpy as np
import matplotlib.pyplot as plt

# -------------------------------
# LOAD DATA
# -------------------------------
data = np.loadtxt("postpro.out", skiprows=1)

step     = data[:, 0]
time     = data[:, 1]
u_center = data[:, 2]
v_center = data[:, 3]
p_center = data[:, 4]
u_max    = data[:, 5]
v_max    = data[:, 6]
cfl      = data[:, 7]

# -------------------------------
# PLOT 1: PRESSURE EVOLUTION
# -------------------------------
plt.figure(figsize=(10, 5))
plt.plot(time, p_center, label="Pressure (center)", color="red")
plt.xlabel("Time")
plt.ylabel("Pressure")
plt.title("Pressure Evolution at Center Point")
plt.grid(True)
plt.legend()

# -------------------------------
# PLOT 2: VELOCITY EVOLUTION (CENTER)
# -------------------------------
plt.figure(figsize=(10, 5))
plt.plot(time, u_center, label="u_center", color="blue")
plt.plot(time, v_center, label="v_center", color="green")
plt.xlabel("Time")
plt.ylabel("Velocity")
plt.title("Velocity at Center Point")
plt.grid(True)
plt.legend()

# -------------------------------
# PLOT 3: MAX VELOCITY (FLOW DEVELOPMENT)
# -------------------------------
plt.figure(figsize=(10, 5))
plt.plot(time, u_max, label="u_max", color="purple")
plt.plot(time, v_max, label="v_max", color="orange")
plt.xlabel("Time")
plt.ylabel("Max Velocity")
plt.title("Maximum Velocity in Domain")
plt.grid(True)
plt.legend()

# -------------------------------
# PLOT 4: CFL (STABILITY CHECK)
# -------------------------------
plt.figure(figsize=(10, 5))
plt.plot(time, cfl, label="CFL", color="black")
plt.axhline(1.0, linestyle="--", color="red", label="Stability limit")
plt.xlabel("Time")
plt.ylabel("CFL number")
plt.title("CFL Evolution")
plt.grid(True)
plt.legend()

plt.show()