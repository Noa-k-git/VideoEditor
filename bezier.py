import time

def d(points, t):
    if len(points) == 1:
        print(points[0])
    else:
        for j in range(len(points)-1,0, -1):
            for i in range(j):
                x = ((1-t) * points[i][0] + t * points[i+1][0])
                y = ((1-t) * points[i][1] + t * points[i+1][1])
                points[i] = (x, y)
        print(points[0])
        
    
def bez(a, b, c, d, t):
    s = 1-t
    ab = a*s + b*t
    bc = b*s + c* t
    cd = c*s + d*t
    abc = ab * s + bc * t
    bcd = bc * s + cd * t
    print(abc*s + bcd * t)

def be(p1, p2, p3, p4, t):
    s = 1- t
    print(p1*(s*s*s) + 3 * p2 * t * (s*s) + 3 * s * (t*t*t) * p3 + t*t*t * p4)

def test(p1, p2, p3, p4):
    start = time.time()
    bez(p1,p2,p3,p4,.6)
    bez(p1,p2,p3,p4,.6)
    bez(p1,p2,p3,p4,.6)
    bez(p1,p2,p3,p4,.6)
    bez(p1,p2,p3,p4,.6)
    bez(p1,p2,p3,p4,.6)
    bez(p1,p2,p3,p4,.6)
    bez(p1,p2,p3,p4,.6)
    bez(p1,p2,p3,p4,.6)
    bez(p1,p2,p3,p4,.6)
    bez(p1,p2,p3,p4,.6)
    bez(p1,p2,p3,p4,.6)
    stop = time.time()
    print("timer:",stop - start)
    start = time.time()
    be(p1,p2,p3,p4,.6)
    be(p1,p2,p3,p4,.6)
    be(p1,p2,p3,p4,.6)
    be(p1,p2,p3,p4,.6)
    be(p1,p2,p3,p4,.6)
    be(p1,p2,p3,p4,.6)
    be(p1,p2,p3,p4,.6)
    be(p1,p2,p3,p4,.6)
    be(p1,p2,p3,p4,.6)
    be(p1,p2,p3,p4,.6)
    be(p1,p2,p3,p4,.6)
    be(p1,p2,p3,p4,.6)
    stop = time.time()
    print("timer:",stop - start)
    # d([(p1, p1), (p2, p2), (p3,p3),(p4,p4)], .6)
    
# p1, p2, p3, p4 = 0, .25, .75, 1
test(0, .25, .75, 1)
# test(0, 0, 1, 1)
