import matplotlib.pyplot as plt
dist= [2817, 5070, 7575, 63,   160,  162,  752,  968,  1755]
bps = [2493, 2493, 1246, 30245,30245,17204,30245,17204,2493]

plt.title("LoRa Performance")
plt.grid(True)
plt.xlabel('Distanse, [m]')
plt.ylabel('DataRate, [Bit/s]')
plt.plot(dist,bps,'x')
plt.show()