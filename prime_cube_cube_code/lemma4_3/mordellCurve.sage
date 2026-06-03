import pickle
possibleCs2, possibleCs3 = pickle.load(open("possibleCs.pkl", "rb"))

def skipResidues(start, end, residues, n):
    i = 0
    m = len(residues)
    if start%n>residues[-1]:
        k= start//n+1
    else:
        while start%n > residues[i] and i<m:
            i+=1
        k = start//n
    val = k*n+residues[i]
    while val <= end:
        yield val
        i = (i+1)%m
        if i == 0:
            k += 1
        val = k*n+residues[i]

def cubic_forms(K, D):
    forms = []

    # irreducible
    a_max = floor( (2/(3*sqrt(3))) * K^(1/4) )
    for a in range(1, int(a_max)+1):
        b_max = floor(a/2 + (1/3)*sqrt(max(0, K^(1/2) - (27/4)*a^2)))
        for b in range(0, int(b_max)+1):
            R.<P> = PolynomialRing(RR)
            poly = -4*P^3 + (3*a+6*b)^2*P^2 + 27*a^2*K
            roots = [r for r in poly.roots(multiplicities=False) if r > 0]
            if not roots:
                continue
            P2 = min(roots)

            cmin = ceil((9*b**2-P2)/(9*a))
            cmax = b-a
            n2, residues2 = possibleCs2[(a%(2**6), b%(2**6))]
            n3, residues3 = possibleCs3[(a%(3**3), b%(3**3))]
            residues = []
            for r2 in residues2:
                for r3 in residues3:
                    residues.append(crt([r2, r3], [n2, n3]))
            residues = sorted(residues)

            for c in skipResidues(cmin, cmax, residues, n2*n3):
                R.<d> = PolynomialRing(ZZ)
                f = -27*(a**2*d**2-6*a*b*c*d-3*b**2*c**2+4*a*c**3+4*b**3*d)+108*D
                roots = [r for r in f.roots(multiplicities=False)]
                for root in roots:
                    forms.append((a, b, c, root, D))
        
    #reducible
    Cmin = ceil(-(K/108)^(1/3))
    Cmax = floor((K/27)^(1/2))
    for C in range(Cmin, Cmax+1):
        R.<B> = PolynomialRing(ZZ)
        f = 27*C**2*(3*B**2-4*C)+108*D
        roots = [r for r in f.roots(multiplicities=False)]
        for root in roots:
            forms.append((1, root, C, 0, D))

    return forms

def mordellCurveIntegerPoints(K):
    integralXs = set()
    for a, b, c, d, D in cubic_forms(-K*108, K):
        t = var("t")
        f = a*t^3 + 3*b*t^2 + 3*c*t + d
        S = gp.thueinit(f, flag=1)
        sols = [tuple(sol) for sol in gp.thue(S, 1)]
        for x, y in sols:
            x, y = int(x), int(y)
            fxx = 6*a*x+6*b*y
            fxy = 6*b*x+6*c*y
            fyy = 6*c*x+6*d*y
            H = fxx*fyy-fxy**2
            assert H%36 == 0
            X = -H//36
            integralXs.add(X)
    
    integerPoints = []
    for x in integralXs:
        print("here", x, x**3+K)
        y = integer_nthroot(x**3+K, 2)[0]
        integerPoints.append((x, y))
        # if y%36 == 0 and x%12 == 0 and y>0 and (y//36)%2==1:
        #     t = (y//36+1)//2
        #     c = x//12
        #     n = e+t**3
        #     if integer_nthroot(n, 3) == t:
        #         possExceptions.append(n)
    return integerPoints