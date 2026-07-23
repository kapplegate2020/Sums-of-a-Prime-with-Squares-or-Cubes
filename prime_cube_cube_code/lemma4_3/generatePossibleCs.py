import pickle

#this function takes in a set of residues mod p^k and reduces it to an
#equivalent set of residues mod p^l for some l<=k
#for example, it would take [0, 1, 16, 17, 32, 33, 48, 49] mod 64 to the list [0, 1] (mod 16)
def reduce(residues, p, k):
    while k>0:
        k-=1
        reduced = {r%(p**k) for r in residues}
        if len(reduced)*p != len(residues):
            return residues, p**(k+1)
        residues = reduced
    return residues, p**k


#we loop through all classes of a, b, c, and d mod base^k to check if the discriminant is divisible by base^k
#for each (a, b) pair, we record each c such that there exists some d with the discriminant divisible by base^k
def getPossibleCs(base, k):
    possibleCs = {}
    for a in range(base**k):
        for b in range(base**k):
            cs = []
            for c in range(base**k):
                for d in range(base**k):
                    D = a**2*d**2-6*a*b*c*d-3*b**2*c**2+4*a*c**3+4*b**3*d
                    if D%(base**k) == 0:
                        cs.append(c)
                        break
            residues, n = reduce(cs, base, k)
            possibleCs[(a, b)] = (n, residues)
    return possibleCs

possibleCs2 = getPossibleCs(2, 6)
possibleCs3 = getPossibleCs(3, 3)


#check what percentage of tuples (a, b, c) mod 2^6 we actually need to search (how much speedup)
total = 0
for a in range(64):
    for b in range(64):
        m, s = possibleCs2[(a, b)]
        total += len(s)*(64//m)
print(total/(64**3))

#check what percentage of tuples (a, b, c) mod 3^3 we actually need to search (how much speedup)
total = 0
for a in range(27):
    for b in range(27):
        m, s = possibleCs3[(a, b)]
        total += len(s)*(27//m)
print(total/(27**3))


pickle.dump((possibleCs2, possibleCs3), open("possibleCs.pkl", "wb"))