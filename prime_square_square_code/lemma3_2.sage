from theorem1_3 import isSquareSquarePrime

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
            if isSquareSquarePrime(n): #check whether the solution really is an exception
                print("worked", n)
            else:
                print("exception", n)

#note that not all (x, y) pairs actually correlate to some n where n-t**2=e and n-(t-i)**2 is_prime
#a square for i=1, 2, 3. 


#Output:
# worked 9
# worked 66625
# worked 985
# worked 2692
# worked 38404
# worked 146512