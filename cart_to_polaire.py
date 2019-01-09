import math
import numpy as np

angle = np.zeros((33,33))

print(angle[0].size)
print(int(angle[0].size/2))
fichier = open("cart_to_polaire_angle.txt", "w")
fichier.write("double angle[33][33] = \n{")
for x in range(-int(angle[0].size/2),int(angle[0].size/2)+1):
    fichier.write("{")
    for y in range(-int(angle[0].size/2),int(angle[0].size/2)+1):
        a = - round((math.atan2(y,x)*180/math.pi)/6)
        if (a < 0):
            a += 60
        angle[x+int(angle[0].size/2),y+int(angle[0].size/2)] = a
        fichier.write(str(a))
        if y != int(angle[0].size/2):
            fichier.write(",")
    if x != int(angle[0].size/2):
        fichier.write("},\n")
    else:
        fichier.write("}\n")
fichier.write("}")
fichier.close()
print(angle)


r = np.zeros((33,33))

fichier = open("cart_to_polaire_r.txt", "w")
fichier.write("double r[33][33] = \n{")
for x in range(-int(r[0].size/2),int(r[0].size/2)+1):
    fichier.write("{")
    for y in range(-int(r[0].size/2),int(r[0].size/2)+1):
        r_value = round(math.sqrt(pow(x,2)+pow(y,2)))
        r[x+int(r[0].size/2),y+int(r[0].size/2)] = r_value
        fichier.write(str(r_value))
        if y != int(r[0].size/2):
            fichier.write(",")
    if x != int(r[0].size/2):
        fichier.write("},\n")
    else:
        fichier.write("}\n")
fichier.write("}")
fichier.close()
print(angle)



