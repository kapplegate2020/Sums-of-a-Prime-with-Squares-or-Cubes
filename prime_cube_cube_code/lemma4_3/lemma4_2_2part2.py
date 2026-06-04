import sys
from pathlib import Path
sys.path.append(str(Path(__file__).resolve().parent.parent))
import helperFunctions

possible_ns = list(map(int, open("logs/possExceptions.txt", "r").read().split()))


with open("possible_ns.txt", "w") as f:
    for x in sorted(possible_ns):
        f.write(f"{x}\n")



definite_ns = set()
for x in possible_ns:
    if not helperFunctions.getCubeCubePrime(x):
        if helperFunctions.verifyException(x):
            definite_ns.add(x)

with open("definite_ns.txt", "w") as f:
    for x in sorted(definite_ns):
        f.write(f"{x}\n")