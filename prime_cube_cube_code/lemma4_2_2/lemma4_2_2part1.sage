from time import time
from pathlib import Path
import shutil
from multiprocessing import Pool
from sympy import integer_nthroot
load("mordellCurve.sage")

exceptions = list(map(int, open("../../exceptions/prime_cube.txt", "r").read().split()))

    

def tryEC(e):
    try:
        possExceptions = []
        a6 = -27*(e+1)
        E = EllipticCurve([0, 0, -9, 0, a6])
        points = E.integral_points(both_signs=True)
        for point in points:
            x, y = int(point[0]), int(point[1])
            if x%3 == 0 and y%9 == 0 and y>0:
                t = y//9
                c = x//3
                n = e+t**3
                if integer_nthroot(n, 3)[0] == t:
                    possExceptions.append(n)
        return possExceptions
    except:
        return None


def doMordellCurve(e):
    print("okkk", e)
    K = 16*27*(-1-4*e)
    integerPoints = mordellCurveIntegerPoints(K)
    possibleExceptions = []
    for x, y in integerPoints:
        if y%36 == 0 and x%12 == 0 and y>0 and (y//36)%2==1:
            t = (y//36+1)//2
            c = x//12
            n = e+t**3
            print(x, y, t, c, n, e)
            if integer_nthroot(n, 3)[0] == t:
                possibleExceptions.append(n)
    return possibleExceptions


def appendFile(filename, message):
    with open(filename, "a") as f:
        f.write(message)


def process(inp):
    i, e = inp
    print(i, e)
    start = time()
    possExceptions = []
    possExceptions = tryEC(e)
    lastTime = time()-start
    if possExceptions == None:
        appendFile(f"temp/log/{i}.txt", f"e={e}, EC failed, time={lastTime}\n")
        start = time()
        possExceptions = doMordellCurve(e)
        lastTime = time()-start
        appendFile(f"temp/log/{i}.txt", f"e={e}, method=MWcurve, possExceptions={possExceptions}, time={lastTime}\n")
    else:
        appendFile(f"temp/log/{i}.txt", f"e={e}, method=EC, possExceptions={possExceptions}, time={lastTime}\n")
    appendFile(f"temp/possExceptionByE/{i}.txt", str(e)+":"+str(possExceptions)+"\n")
    for exception in possExceptions:
        appendFile(f"temp/possExceptions/{i}.txt", str(exception)+" ")




totalToDo = len(exceptions)
lastTime = 0
Path("temp").mkdir(exist_ok=True)
Path("temp/log").mkdir(exist_ok=True)
Path("temp/possExceptionByE").mkdir(exist_ok=True)
Path("temp/possExceptions").mkdir(exist_ok=True)
with Pool() as p:
    p.map(process, enumerate(exceptions[:200]+[36261]))


try:
    shutil.rmtree("logs")
except:
    pass
Path("logs").mkdir(exist_ok=True)
for entry in sorted(Path("temp/log").iterdir(), key=lambda p: int(p.stem)):
    if entry.is_file():
        content = entry.read_text()
        appendFile("logs/log.txt", content)
for entry in sorted(Path("temp/possExceptionByE").iterdir(), key=lambda p: int(p.stem)):
    if entry.is_file():
        content = entry.read_text()
        appendFile("logs/possExceptionByE.txt", content)
for entry in sorted(Path("temp/possExceptions").iterdir(), key=lambda p: int(p.stem)):
    if entry.is_file():
        content = entry.read_text()
        appendFile("logs/possExceptions.txt", content)


shutil.rmtree("temp")
