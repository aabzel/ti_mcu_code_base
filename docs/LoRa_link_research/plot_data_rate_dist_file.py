import matplotlib.pyplot as plt

file = open('LoRa_link_dist_rate.txt')
content = file.readlines()

plt.title("LoRa Performance")
plt.grid(True)
plt.xlabel('Distance, [m]')
plt.ylabel('DataRate, [Bit/s]')

# loop over lines in a file
for pos, a_string in enumerate(content):
    # print the required line number
    #print(a_string)
    a_list = a_string.split()
    map_object = map(float, a_list)
    list_of_integers = list(map_object)
    print(list_of_integers)
    plt.plot(list_of_integers[0], list_of_integers[1], 'ro')
    
plt.show()
        
        
