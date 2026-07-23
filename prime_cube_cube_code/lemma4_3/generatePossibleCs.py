import pickle

def reduce(residues, p, k):
    while k>0:
        k-=1
        reduced = {r%(p**k) for r in residues}
        if len(reduced)*p != len(residues):
            return residues, p**(k+1)
        residues = reduced
    return residues, p**k


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

total = 0
for a in range(2**6):
    for b in range(2**6):
        m, s = possibleCs2[(a, b)]
        total += len(s)*(64//m)
print(total/(64**3))

#pickle.dump((possibleCs2, possibleCs3), open("possibleCs.pkl", "wb"))