\ENCODING=ISO-8859-1
\Problem name: IloCplex

Minimize
 obj1: 2.04576519233974 arc0(1) + 2.732421875 arc0(3)
       + 2.04576519233974 arc1(0) + 2.04576519233974 arc1(2)
       + 2.732421875 arc1(4) + 2.04576519233974 arc2(1) + 2.732421875 arc2(5)
       + 2 arc3(0) + 2.04578754408162 arc3(4) + 2.732421875 arc3(6) + 2 arc4(1)
       + 2.04578754408162 arc4(3) + 2.04578754408162 arc4(5)
       + 2.732421875 arc4(7) + 2 arc5(2) + 2.04578754408162 arc5(4)
       + 2.732421875 arc5(8) + 2 arc6(3) + 2 arc6(7) + 2 arc7(4) + 2 arc7(6)
       + 2 arc7(8) + 2 arc8(5) + 2 arc8(7)
Subject To
 c1:  - arc0(1) - arc0(2) - arc0(3) - arc0(4) - arc0(5) - arc0(6) - arc0(7)
      - arc0(8) + arc1(0) + arc2(0) + arc3(0) + arc4(0) + arc5(0) + arc6(0)
      + arc7(0) + arc8(0)  = 0
 c2:  arc0(1) - arc1(0) - arc1(2) - arc1(3) - arc1(4) - arc1(5) - arc1(6)
      - arc1(7) - arc1(8) + arc2(1) + arc3(1) + arc4(1) + arc5(1) + arc6(1)
      + arc7(1) + arc8(1)  = 0
 c3:  - arc0(2) - arc1(2) + arc2(0) + arc2(1) + arc2(3) + arc2(4) + arc2(5)
      + arc2(6) + arc2(7) + arc2(8) - arc3(2) - arc4(2) - arc5(2) - arc6(2)
      - arc7(2) - arc8(2)  = 1
 c4:  arc0(3) + arc1(3) + arc2(3) - arc3(0) - arc3(1) - arc3(2) - arc3(4)
      - arc3(5) - arc3(6) - arc3(7) - arc3(8) + arc4(3) + arc5(3) + arc6(3)
      + arc7(3) + arc8(3)  = 0
 c5:  arc0(4) + arc1(4) + arc2(4) + arc3(4) - arc4(0) - arc4(1) - arc4(2)
      - arc4(3) - arc4(5) - arc4(6) - arc4(7) - arc4(8) + arc5(4) + arc6(4)
      + arc7(4) + arc8(4)  = 0
 c6:  arc0(5) + arc1(5) + arc2(5) + arc3(5) + arc4(5) - arc5(0) - arc5(1)
      - arc5(2) - arc5(3) - arc5(4) - arc5(6) - arc5(7) - arc5(8) + arc6(5)
      + arc7(5) + arc8(5)  = 0
 c7:  arc0(7) + arc1(7) + arc2(7) + arc3(7) + arc4(7) + arc5(7) + arc6(7)
      - arc7(0) - arc7(1) - arc7(2) - arc7(3) - arc7(4) - arc7(5) - arc7(6)
      - arc7(8) + arc8(7)  = 0
 c8:  arc0(8) + arc1(8) + arc2(8) + arc3(8) + arc4(8) + arc5(8) + arc6(8)
      + arc7(8) - arc8(0) - arc8(1) - arc8(2) - arc8(3) - arc8(4) - arc8(5)
      - arc8(6) - arc8(7)  = 0
 c9:  arc0(6) + arc1(6) + arc2(6) + arc3(6) + arc4(6) + arc5(6) + arc6(6)
      + arc7(6) + arc8(6)  = 1
 c10: arc0(0) <= 0
 c11: arc0(1) <= 8
 c12: arc0(2) <= 0
 c13: arc0(3) <= 8
 c14: arc0(4) <= 0
 c15: arc0(5) <= 0
 c16: arc0(6) <= 0
 c17: arc0(7) <= 0
 c18: arc0(8) <= 0
 c19: arc1(0) <= 8
 c20: arc1(1) <= 0
 c21: arc1(2) <= 8
 c22: arc1(3) <= 0
 c23: arc1(4) <= 0
 c24: arc1(5) <= 0
 c25: arc1(6) <= 0
 c26: arc1(7) <= 0
 c27: arc1(8) <= 0
 c28: arc2(0) <= 0
 c29: arc2(1) <= 8
 c30: arc2(2) <= 0
 c31: arc2(3) <= 0
 c32: arc2(4) <= 0
 c33: arc2(5) <= 8
 c34: arc2(6) <= 0
 c35: arc2(7) <= 0
 c36: arc2(8) <= 0
 c37: arc3(0) <= 8
 c38: arc3(1) <= 0
 c39: arc3(2) <= 0
 c40: arc3(3) <= 0
 c41: arc3(4) <= 0
 c42: arc3(5) <= 0
 c43: arc3(6) <= 0
 c44: arc3(7) <= 0
 c45: arc3(8) <= 0
 c46: arc4(0) <= 0
 c47: arc4(1) <= 8
 c48: arc4(2) <= 0
 c49: arc4(3) <= 8
 c50: arc4(4) <= 0
 c51: arc4(5) <= 8
 c52: arc4(6) <= 0
 c53: arc4(7) <= 8
 c54: arc4(8) <= 0
 c55: arc5(0) <= 0
 c56: arc5(1) <= 0
 c57: arc5(2) <= 8
 c58: arc5(3) <= 0
 c59: arc5(4) <= 8
 c60: arc5(5) <= 0
 c61: arc5(6) <= 0
 c62: arc5(7) <= 0
 c63: arc5(8) <= 0
 c64: arc6(0) <= 0
 c65: arc6(1) <= 0
 c66: arc6(2) <= 0
 c67: arc6(3) <= 8
 c68: arc6(4) <= 0
 c69: arc6(5) <= 0
 c70: arc6(6) <= 0
 c71: arc6(7) <= 8
 c72: arc6(8) <= 0
 c73: arc7(0) <= 0
 c74: arc7(1) <= 0
 c75: arc7(2) <= 0
 c76: arc7(3) <= 0
 c77: arc7(4) <= 8
 c78: arc7(5) <= 0
 c79: arc7(6) <= 8
 c80: arc7(7) <= 0
 c81: arc7(8) <= 8
 c82: arc8(0) <= 0
 c83: arc8(1) <= 0
 c84: arc8(2) <= 0
 c85: arc8(3) <= 0
 c86: arc8(4) <= 0
 c87: arc8(5) <= 8
 c88: arc8(6) <= 0
 c89: arc8(7) <= 8
 c90: arc8(8) <= 0
Bounds
 0 <= arc0(0) <= 1
 0 <= arc0(1) <= 1
 0 <= arc0(2) <= 1
 0 <= arc0(3) <= 1
 0 <= arc0(4) <= 1
 0 <= arc0(5) <= 1
 0 <= arc0(6) <= 1
 0 <= arc0(7) <= 1
 0 <= arc0(8) <= 1
 0 <= arc1(0) <= 1
 0 <= arc1(1) <= 1
 0 <= arc1(2) <= 1
 0 <= arc1(3) <= 1
 0 <= arc1(4) <= 1
 0 <= arc1(5) <= 1
 0 <= arc1(6) <= 1
 0 <= arc1(7) <= 1
 0 <= arc1(8) <= 1
 0 <= arc2(0) <= 1
 0 <= arc2(1) <= 1
 0 <= arc2(2) <= 1
 0 <= arc2(3) <= 1
 0 <= arc2(4) <= 1
 0 <= arc2(5) <= 1
 0 <= arc2(6) <= 1
 0 <= arc2(7) <= 1
 0 <= arc2(8) <= 1
 0 <= arc3(0) <= 1
 0 <= arc3(1) <= 1
 0 <= arc3(2) <= 1
 0 <= arc3(3) <= 1
 0 <= arc3(4) <= 1
 0 <= arc3(5) <= 1
 0 <= arc3(6) <= 1
 0 <= arc3(7) <= 1
 0 <= arc3(8) <= 1
 0 <= arc4(0) <= 1
 0 <= arc4(1) <= 1
 0 <= arc4(2) <= 1
 0 <= arc4(3) <= 1
 0 <= arc4(4) <= 1
 0 <= arc4(5) <= 1
 0 <= arc4(6) <= 1
 0 <= arc4(7) <= 1
 0 <= arc4(8) <= 1
 0 <= arc5(0) <= 1
 0 <= arc5(1) <= 1
 0 <= arc5(2) <= 1
 0 <= arc5(3) <= 1
 0 <= arc5(4) <= 1
 0 <= arc5(5) <= 1
 0 <= arc5(6) <= 1
 0 <= arc5(7) <= 1
 0 <= arc5(8) <= 1
 0 <= arc6(0) <= 1
 0 <= arc6(1) <= 1
 0 <= arc6(2) <= 1
 0 <= arc6(3) <= 1
 0 <= arc6(4) <= 1
 0 <= arc6(5) <= 1
 0 <= arc6(6) <= 1
 0 <= arc6(7) <= 1
 0 <= arc6(8) <= 1
 0 <= arc7(0) <= 1
 0 <= arc7(1) <= 1
 0 <= arc7(2) <= 1
 0 <= arc7(3) <= 1
 0 <= arc7(4) <= 1
 0 <= arc7(5) <= 1
 0 <= arc7(6) <= 1
 0 <= arc7(7) <= 1
 0 <= arc7(8) <= 1
 0 <= arc8(0) <= 1
 0 <= arc8(1) <= 1
 0 <= arc8(2) <= 1
 0 <= arc8(3) <= 1
 0 <= arc8(4) <= 1
 0 <= arc8(5) <= 1
 0 <= arc8(6) <= 1
 0 <= arc8(7) <= 1
 0 <= arc8(8) <= 1
Binaries
 arc0(0)  arc0(1)  arc0(2)  arc0(3)  arc0(4)  arc0(5)  arc0(6)  arc0(7) 
 arc0(8)  arc1(0)  arc1(1)  arc1(2)  arc1(3)  arc1(4)  arc1(5)  arc1(6) 
 arc1(7)  arc1(8)  arc2(0)  arc2(1)  arc2(2)  arc2(3)  arc2(4)  arc2(5) 
 arc2(6)  arc2(7)  arc2(8)  arc3(0)  arc3(1)  arc3(2)  arc3(3)  arc3(4) 
 arc3(5)  arc3(6)  arc3(7)  arc3(8)  arc4(0)  arc4(1)  arc4(2)  arc4(3) 
 arc4(4)  arc4(5)  arc4(6)  arc4(7)  arc4(8)  arc5(0)  arc5(1)  arc5(2) 
 arc5(3)  arc5(4)  arc5(5)  arc5(6)  arc5(7)  arc5(8)  arc6(0)  arc6(1) 
 arc6(2)  arc6(3)  arc6(4)  arc6(5)  arc6(6)  arc6(7)  arc6(8)  arc7(0) 
 arc7(1)  arc7(2)  arc7(3)  arc7(4)  arc7(5)  arc7(6)  arc7(7)  arc7(8) 
 arc8(0)  arc8(1)  arc8(2)  arc8(3)  arc8(4)  arc8(5)  arc8(6)  arc8(7) 
 arc8(8) 
End
