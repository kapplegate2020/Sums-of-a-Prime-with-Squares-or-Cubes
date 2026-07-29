from theorem1_3 import isSquareSquarePrime
from sympy import integer_nthroot

exceptions = list(map(int, open("../exceptions/prime_square.txt", "r").read().split()))


for e in exceptions:
    a2 = 11*e-36
    a4 = 3*(12*e**2-96*e+132)
    a6 = 36*(e**3-14*e**2+49*e-36)
    E = EllipticCurve([0, a2, 0, a4, a6])
    points = E.integral_points(both_signs=True)
    for point in points:
        x, y_ = int(point[0]), int(point[1])
        if x%12 == 0 and y_%6 == 0 and y_>0: #verify that the solution might be an exception
            t = x//12
            n = t**2+e
            realT = integer_nthroot(n, 2)[0]
            check1 = (n-realT**2 == e)
            check2 = integer_nthroot(n-(realT-1)**2, 2)[1]
            check3 = integer_nthroot(n-(realT-2)**2, 2)[1]
            check4 = integer_nthroot(n-(realT-3)**2, 2)[1]
            if check1 and check2 and check3 and check4:
                print(n)

#note that not all (x, y) pairs actually correlate to some n where n-t**2=e and n-(t-i)**2 is_prime
#a square for i=1, 2, 3. 


# Output:
# 66625