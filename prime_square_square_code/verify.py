primes = set(map(int, open("../../parallel/primes/primes6.txt", "r").read().split()[1:]))

def getSquareRootFloor(m):
    low = 1
    high = m
    while True:
        guess = (low+high)//2
        if guess*guess == m:
            return guess
        if high<=low:
            if low*low < m:
                return low
            return low-1
        if guess*guess < m:
            low = guess+1
        if guess*guess > m:
            high = guess - 1

def verify(n):
    t=getSquareRootFloor(n)
    while t>0:
        n1 = n-t**2
        if t<=0:
            return False
        t1 = getSquareRootFloor(n1)
        for _ in range(100):
            p = n1-t1**2
            if t1<=0:
                break
            if p in primes:
                return True
            t1-=1
        t-=1
    return False
