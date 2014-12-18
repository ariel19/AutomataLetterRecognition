import os

for val in range(1, 106, 5):
	cmd = "python TAIOa.py -etap a1 -wejscieTyp gen -PSOiter 100 -PSOs 24 -procRozmTest 20 -iloscKlas 10 -iloscCech 16 -iloscPowtorzenWKlasie " + str(val) +  " -minLos 0 -maxLos 20 -zaburzenie 1 -dyskretyzacja 5"
	os.system(cmd)
