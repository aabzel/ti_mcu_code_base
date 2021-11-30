
import math
R=6371000
h=3.2
print ('h {} m'.format(h))
L=math.sqrt(((R+h)**2)-(R**2))
print ('L {} m'.format(L))

desL = 5500
print ('L {} m '.format(desL))
H = math.sqrt((desL**2)+(R**2))-R
print ('H {} m or {} floors'.format(H,H/3))
#на какую высоту надо поднять антенну чтобы была видимость 10 км?