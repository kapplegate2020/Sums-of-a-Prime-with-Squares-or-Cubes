from sympy import integer_nthroot

#implementation is taken from https://gist.github.com/Ayrx/5884790 with edits to make it deterministic
def deterministicMillerRabin(p):
    if p == 1:
        return False
    if p in [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41]:
        return True
    if p%2 == 0:
        return False
    if p >= 3317044064679887385961981:
        return False

    u = p-1
    t = 0
    while u%2 == 0:
        u//= 2
        t+=1
    
    for a in [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41]:
        if a%p == 0:
            return False
        aPower = pow(a, u, p)

        #if we start with a 1 or -1, then N could be prime
        if aPower in [p-1, 1]: 
            continue

        #if at any point we hit -1, the test is inclusive.
        #if we hit 1 and didn't hit a -1 before, N is composite
        testInconlusive = False
        for _ in range(t-1):
            aPower = pow(aPower, 2, p)
            if aPower == p-1:
                testInconlusive = True
                break
            if aPower == 1:
                return False
        if testInconlusive:
            continue

        #if a^((N-1)/2)%N != -1 then either a^(N-1)%N!=1 or, a^((N-1)/2)%N is a nontrivial root of 1
        #in either case, N is composite
        return False
    
    return True

#we just do the greedy algorithm, subtracting off the biggest squares to see if what is left is prime
#counter is used to move on to a more likely guess for t if it fails several times
#this test may return None when there is a solution
def getCubeCubePrime(n):
    t = integer_nthroot(n, 3)[0]
    while t>0:
        n1 = n-t**3
        t1 = integer_nthroot(n1, 3)[0]
        counter = 0
        while t1>0 and counter<100:
            counter+=1
            res = n1-t1**3
            if deterministicMillerRabin(res) and res!=2:
                return t, t1, res
            t1-= 1
        t-=1
    return None

#this test verifies that there is no solution, but may take longer to find a solution if one is present
def verifyException(n):
    t = integer_nthroot(n, 3)[0]
    while t>0:
        n1 = n-t**3
        t1 = integer_nthroot(n1, 3)[0]
        while t1>0:
            res = n1-t1**3
            if deterministicMillerRabin(res) and res!=2:
                return False
            t1-= 1
        t-=1
    return True