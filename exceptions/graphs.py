square = list(map(int, open("prime_square.txt", "r").read().split()))
cube = list(map(int, open("prime_cube.txt", "r").read().split()))

print(square[0], square[-1])
print(cube[0], cube[-1])

cuts = [10000, 100000, 1000000, 10000000, 100000000]
sCounts = [0 for _ in cuts]
cCounts = [0 for _ in cuts]

i=0
for s in square:
    if s>cuts[i]:
        i+=1
    sCounts[i]+=1

i=0
for c in cube:
    if c>cuts[i]:
        i+=1
    cCounts[i]+=1

print(sCounts)
print(cCounts)
print(len(square))
print(len(cube))