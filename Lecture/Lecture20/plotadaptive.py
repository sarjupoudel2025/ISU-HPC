import numpy as np
import matplotlib.pyplot as plt

# ==============================
# Plot V(x0)
# ==============================
data_V = np.loadtxt("Vx0.dat")

# 🔥 SORT by x0 (CRITICAL FIX)
data_V = data_V[data_V[:, 0].argsort()]

x0 = data_V[:, 0]
V = data_V[:, 1]

plt.figure()
plt.plot(x0, V, linewidth=2)
plt.xlabel("x0")
plt.ylabel("V(x0)")
plt.title("Electrostatic Potential V(x0)")
plt.grid(True, linestyle="--", alpha=0.6)
plt.savefig("Vx0.png")



# ==============================
# Plot J0(x)
# ==============================
data_J = np.loadtxt("J0.dat")

# 🔥 SORT by x (CRITICAL FIX)
data_J = data_J[data_J[:, 0].argsort()]

x = data_J[:, 0]
J0 = data_J[:, 1]

plt.figure()
plt.plot(x, J0, linewidth=2)
plt.xlabel("x")
plt.ylabel("J0(x)")
plt.title("Bessel Function J0(x)")
plt.grid(True, linestyle="--", alpha=0.6)
plt.savefig("J0.png")
plt.show()