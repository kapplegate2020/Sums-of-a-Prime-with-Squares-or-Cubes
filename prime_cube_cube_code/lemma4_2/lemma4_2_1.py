import sys
from pathlib import Path
sys.path.append(str(Path(__file__).resolve().parent.parent)) #get access helperFunctions.py from the parent directory
import helperFunctions

#compute all the outputs to the polynomial 3t^3-3t+1 and store them in a dict
exceptionList = list(map(int, open("../../exceptions/prime_cube.txt", "r").read().split()))
polynomialValues = {}
t = 1
val = 1
while val < exceptionList[-1]:
    polynomialValues[val] = t
    t+=1
    val = 3*t**2-3*t+1

possible_ns = set()

#for every pair of exceptions, check if their difference is an output to the polynomial
for i, e1 in enumerate(exceptionList):
    for e2 in exceptionList[i:]:
        if e2-e1 in polynomialValues:
            possible_ns.add(polynomialValues[e2-e1]**3+e1)

#write ns that may be prime-cube-cube exceptions to file
with open("lemma4_2_1_possible_ns.txt", "w") as f:
    for x in sorted(possible_ns):
        f.write(f"{x}\n")


#check to see if these are really prime-cube-cube exceptions
definite_ns = set()
for x in possible_ns:
    if not helperFunctions.getCubeCubePrime(x):
        if helperFunctions.verifyException(x):
            definite_ns.add(x)

#write ns that are prime-cube-cube exceptions to file
with open("lemma4_2_1_definite_ns.txt", "w") as f:
    for x in sorted(definite_ns):
        f.write(f"{x}\n")