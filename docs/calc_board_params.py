
XTAL=48000000
clk_Period = 1/XTAL
ticPerUs = (10**(-6))/clk_Period
Period32bitTimer = (2**32)*(10**(-6))
Period32bit1msTimer = (2**32)*(10**(-3))
minCntPer=clk_Period
maxCntPer=clk_Period*255
msInHour = 1000*60*60
KL30_P = 12
R34=27000
R31=270000
PwrVolDivCurr=KL30_P/(R31+R34)
KL30_ADC = PwrVolDivCurr*R34
PwrVolDiv = KL30_P/KL30_ADC
R44= 210000
R39=100000
V_BATT = 4.3
Ibatt=V_BATT/(R44+R39)
BATT_ADC = Ibatt*R44;
BatVolDiv=V_BATT/BATT_ADC
print ('BatVolDiv {}'.format(BatVolDiv))
print ('PwrVolDiv {}'.format(PwrVolDiv))
print ('KL30_P {} V'.format(KL30_P))
print ('PwrVolDivCurr {} A'.format(PwrVolDivCurr))
print ('KL30_ADC {} Hz'.format(KL30_ADC))

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