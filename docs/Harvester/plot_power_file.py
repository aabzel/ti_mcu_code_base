import matplotlib.pyplot as plt

file = open('power.txt')
content = file.readlines()

plt.title("Harvester Power")
plt.grid(True)
plt.xlabel('Voltage, [V]')
plt.ylabel('Current, [A]')

# loop over lines in a file
for pos, a_string in enumerate(content):
    # print the required line number
    #print(a_string)
    a_list = a_string.split()
    map_object = map(float, a_list)
    list_of_integers = list(map_object)
    print(list_of_integers)
    plt.plot(list_of_integers[0], list_of_integers[1], 'ro',label='current')
    plt.plot(list_of_integers[0], list_of_integers[0]*list_of_integers[1], 'rx',label='power')
    plt.legend(['Current,A', 'Power,W'])
    
plt.show()
        
        
