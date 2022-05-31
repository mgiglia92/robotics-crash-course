from math_utils import EulerIntegrator
import numpy as np
import matplotlib.pyplot as plt

# Num samples
N = 10
# Time array
t = np.linspace(0,10,N)
# Function array
x = np.sin(t)
# Integral output as array
integral_trap = np.zeros(N)
integral_right = np.zeros(N)

intg = EulerIntegrator(initial_value=x[0])

for i in range(N):
    if i == 0:
        integral_trap[i]=0
        integral_right[i]=0
    else:
        integral_trap[i] = intg.integrate(x[i], t[i]-t[i-1], type='trap')
        integral_right[i] = intg.integrate(x[i], t[i]-t[i-1], type='right')


plt.figure(1)
plt.plot(t, x, label='f(t)')
plt.plot(t, integral_trap, label='intg_trap')
plt.plot(t, integral_right, label='intg_right')
plt.legend()
plt.show()