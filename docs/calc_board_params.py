
XTAL=48000000
clk_Period = 1/XTAL
ticPerUs = (10**(-6))/clk_Period
Period32bitTimer = (2**32)*(10**(-6))
Period32bit1msTimer = (2**32)*(10**(-3))
minCntPer=clk_Period
maxCntPer=clk_Period*255
msInHour = 1000*60*60


print ('msInHour {} ms'.format(msInHour))
print ('Xtall {} Hz'.format(XTAL))
print ('clk_Period {} s'.format(clk_Period))
print ('ticPerUs {}'.format(ticPerUs))
print ('Period of 32 bit 1us timer {} s'.format(Period32bitTimer))
print ('Period of 32 bit 1us timer {} min'.format(Period32bitTimer/60))
print ('Period of 32 bit 1us timer {} h'.format(Period32bitTimer/(60*60)))
print ('minCntPer {} s'.format(minCntPer))
print ('maxCntPer {} s'.format(maxCntPer))
print ('Period of 32 bit 1ms timer {} s'.format(Period32bit1msTimer))
print ('Period of 32 bit 1ms timer {} min'.format(Period32bit1msTimer/60))
print ('Period of 32 bit 1ms timer {} h'.format(Period32bit1msTimer/(60*60)))
print ('Period of 32 bit 1ms timer {} day'.format(Period32bit1msTimer/(60*60*24)))