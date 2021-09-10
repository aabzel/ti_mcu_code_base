
XTAL=48000000
clk_Period = 1/XTAL
ticPerUs = (10**(-6))/clk_Period
print ('Xtall {}Hz'.format(XTAL))
print ('clk_Period {}s'.format(clk_Period))
print ('ticPerUs {}'.format(ticPerUs))