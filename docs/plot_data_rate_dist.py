import matplotlib.pyplot as plt
dist= [63,   160,  752,  968,  1755]
bps = [39062,39062,39062,23437,3417]

plt.title("LoRa Performance")
plt.grid(True)
plt.xlabel('Distanse, [m]')
plt.ylabel('DataRate, [Bit/s]')
plt.plot(dist,bps,'x')
plt.show()