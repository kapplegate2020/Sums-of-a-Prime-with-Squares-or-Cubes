from multiprocessing import Pool, cpu_count
from sympy import integer_nthroot

primes = set(map(int, open("../../parallel/primes/primes6.txt", "r").read().split()[1:]))


def isSquareSquarePrime(n):
    t=integer_nthroot(n, 2)[0]
    while t>0:
        n1 = n-t**2
        if t<=0:
            return False
        t1 = integer_nthroot(n1, 2)[0]
        for _ in range(100):
            p = n1-t1**2
            if t1<=0:
                break
            if p in primes:
                return True
            t1-=1
        t-=1
    return False



def worker(start_end):
    start, end = start_end
    bad = []

    for i in range(start, end):
        if not isSquareSquarePrime(i):
            bad.append(i)

    return bad



N = 120_000_000
chunk = 500_000

ranges = [(i, min(i+chunk, N+1)) for i in range(1, N+1, chunk)]

with Pool(cpu_count()) as p:
    results = p.map(worker, ranges)

for r in results:
    for x in r:
        print(x)


# Output:
# 1
# 2
# 3
# 4
# 6
# 14