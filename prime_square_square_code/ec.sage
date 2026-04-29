from verify import verify

exceptions = list(map(int, open("../exceptions/prime_square.txt", "r").read().split()))


for e in exceptions:
    #print(f"ec {e}:")
    a2 = 11*e-36
    a4 = 3*(12*e**2-96*e+132)
    a6 = 36*(e**3-14*e**2+49*e-36)
    E = EllipticCurve([0, a2, 0, a4, a6])
    points = E.integral_points(both_signs=True)
    for point in points:
        x, y_ = int(point[0]), int(point[1])
        if x%12 == 0 and y_%6 == 0 and y_>0:
            t = x//12
            n = t**2+e
            if verify(n):
                print("worked", n)
            else:
                print("exception", n)

#Output:
# worked 9
# worked 66625
# worked 985
# worked 2692
# worked 38404
# worked 146512