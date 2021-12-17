import matplotlib.pyplot as plt

file = open('gnss_out_m.txt')
content = file.readlines()

#print ('BatVolDiv {}'.format(BatVolDiv))
plt.title("GNSS Path")
plt.grid(True)
plt.xlabel('Lon, [m]') #долгота
plt.ylabel('Lat, [m]') #Широта

# loop over lines in a file
for pos, a_string in enumerate(content):
    # print the required line number
    #print(a_string)
    a_list = a_string.split()
    map_object = map(float, a_list)
    list_of_integers = list(map_object)
    #print(list_of_integers)
    plt.plot(list_of_integers[0], list_of_integers[1], '.', color="blue",linewidth=1)

plt.show()
plt.axis('scaled')
plt.axis('equal')
        

