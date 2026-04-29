from multiprocessing import Pool, cpu_count
from verify import verify



def worker(start_end):
    start, end = start_end
    bad = []

    for i in range(start, end):
        if not verify(i):
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