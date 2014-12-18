import sys
import os

error_fn = "output_err_test.dat"

args_to_script = dict()

# specific for bullet
error = 0.0
bullet_val = 0

def clean_args_info():
	global args_to_script
	args_to_script["-etap"] = False
	args_to_script["-wejscieTyp"] = False
	args_to_script["-PSOiter"] = False
	args_to_script["-PSOs"] = False
	args_to_script["-procRozmTest"] = False
	args_to_script["-iloscKlas"] = False
	args_to_script["-iloscCech"] = False
	args_to_script["-iloscPowtorzenWKlasie"] = False
	args_to_script["-minLos"] = False
	args_to_script["-maxLos"] = False
	args_to_script["-zaburzenie"] = False
	args_to_script["-dyskretyzacja"] = False

clean_args_info()

# specific variables
val1 = "-iloscKlas"
val2 = "-iloscPowtorzenWKlasie"

bul1 = 0
bul2 = 0

cmd_line = "python TAIO2014.py "
# check for valuable args
for i in range(len(sys.argv)):
	arg = sys.argv[i]
	# print arg
	if arg in args_to_script:
		args_to_script[arg] = True

	if arg != "TAIOa.py":
		cmd_line += (arg 	 + ' ')

	# for a) bullet calculate instance number
	if arg == val1:
		bul1 = int(sys.argv[i + 1])
		print sys.argv[i + 1]
	elif arg == val2:
		bul2 = int(sys.argv[i + 1])
		print sys.argv[i + 1]

# calculate bullet value
bullet_val = bul1 * bul2

# check if all params are present
for k, v in args_to_script.items():
	if v != True:
		print "missing: ", k
		sys.exit(127)


# execute Mateusz script
os.system(cmd_line)
	
with open(error_fn, "r") as f:
	nums = f.read().split(' ')
	error = float(nums[0]) / int(nums[1])

os.system("rm input.dat output_class.dat output_err_test.dat output_err_train.dat 2>/dev/null")

bullet_x = "bullet_a"
with open(bullet_x, "a") as f:
	line = str(bullet_val) + ' ' + str(error) + "\n"
	f.write(line)
