from sympy import integer_nthroot
N = 10**17
T = integer_nthroot(N, 3)[0]



for t in range(2, T):
    prev = mid
    mid = next
    next = set()
    max = 9*t**2
    a=1
    while a**3<max:
        m = t**3+a**3
        if m in prev:
            c, _ = integer_nthroot(m-(t-2)**3, 3)
            print(f"n={m}, n^1/3={int(m**(1/3))}, t={t}, t^2/3 = {int(t**(2/3))} a={a}, c={c}, k={c-a}")
        next.add(m)
        a+=1