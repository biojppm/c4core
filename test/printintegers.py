

def nb(val):
    return [val-1, val, val+1]


def ipowers2(min, max):
    vals = []
    v = int(min / 2)
    while v < -10:
        vals += nb(v)
        v = int(v / 2)
    vals += upowers2(max)
    return vals


def upowers2(max):
    vals = []
    v = 16
    while v < max:
        vals += nb(v)
        v *= 2
    return vals


def ipowers10(min, max):
    vals = []
    v = -100
    while v > min:
        vals += nb(v)
        v *= 10
    vals += upowers10(max)
    return vals


def upowers10(max):
    vals = []
    v = 10
    while v < max:
        vals += nb(v)
        v *= 10
    return vals


def idiv10(min, max):
    vals = []
    v = int(min / 10)
    while v < -10:
        vals.append(v)
        v = int(v / 10)
    vals += udiv10(max)
    return vals


def udiv10(max):
    vals = []
    v = int(max / 10)
    while v > 10:
        vals += nb(v)
        v = int(v / 10)
    return vals


def ivals(bits):
    min = -(1 << (bits-1))
    max = -min-1
    vals = [min, min+1, min+2, min+3, min+4, min+5]
    vals += ipowers2(min, max)
    vals += ipowers10(min, max)
    vals += idiv10(min, max)
    vals += [-11, -10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11]
    vals += [max-5, max-4, max-3, max-2, max-1, max]
    return sorted(list(set(vals)))


def uvals(bits):
    max = (1 << bits) - 1
    vals = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11]
    vals += upowers2(max)
    vals += upowers10(max)
    vals += udiv10(max)
    vals += [max-5, max-4, max-3, max-2, max-1, max]
    return sorted(list(set(vals)))



def p(v):
    return f'        nc({v}, "{v}", "{hex(v)}", "{oct(v)}", "{bin(v)}"),'


def pn(numbits, fn):
    print()
    for a in fn(numbits):
        print(p(a))


pn(8, ivals)
pn(8, uvals)

pn(16, ivals)
pn(16, uvals)

pn(32, ivals)
pn(32, uvals)

pn(64, ivals)
pn(64, uvals)
