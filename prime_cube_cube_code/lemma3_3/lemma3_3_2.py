import sys
from pathlib import Path
sys.path.append(str(Path(__file__).resolve().parent.parent)) #get access helperFunctions.py from the parent directory
import helperFunctions

#load in the prime-square exceptions and precompute a set of cubes
exceptionList = list(map(int, open("../../exceptions/prime_cube.txt", "r").read().split()))
cubes = set(t**3 for t in range(0, int(exceptionList[-1]**(1/3))+2))

#compute the largest t such that 3t^2-3t+1 <= e for all e in ex_3
maxT = 0
val = 1
while val < exceptionList[-1]:
    maxT+=1
    val = 3*maxT**2-3*maxT+1

#for all possible t, check if (t-1)^3+e-t^3 is a cube
possible_ns = set()
for t in range(1, maxT+1):
    for e in exceptionList:
        if (t-1)**3+e-t**3 in cubes:
            possible_ns.add((t-1)**3+e)

#write ns that may be prime-cube-cube exceptions to file
with open("lemma4_2_2_possible_ns.txt", "w") as f:
    for x in sorted(possible_ns):
        f.write(f"{x}\n")


#go through all possible ns and check if they are prime-cube-cube exceptions
definite_ns = set()
for x in possible_ns:
    if not helperFunctions.getCubeCubePrime(x):
        if helperFunctions.verifyException(x):
            definite_ns.add(x)

#write ns thar are prime-cube-cube exceptions to file
with open("lemma4_2_2_definite_ns.txt", "w") as f:
    for x in sorted(definite_ns):
        f.write(f"{x}\n")