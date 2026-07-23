import helperFunctions

#we compiles the lists of bad Ns from the lemmas
badNs = list(map(int, open(f"lemma4_2/lemma4_2_1_definite_ns.txt", "r").read().split()))
badNs += list(map(int, open(f"lemma4_2/lemma4_2_2_definite_ns.txt", "r").read().split()))
badNs += list(map(int, open(f"lemma4_3/definite_ns.txt", "r").read().split()))


#the above arguments do not work for t-1<=0
for x in range(1, 8):
    if helperFunctions.verifyException(x):
        badNs.append(x)

#we print the list of exceptions
with open("../exceptions/prime_cube_cube.txt", "w") as f:
    for x in sorted(set(badNs)):
        f.write(f"{x}\n")
