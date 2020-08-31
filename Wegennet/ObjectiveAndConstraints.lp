\ENCODING=ISO-8859-1
\Problem name: IloCplex

Minimize
 obj1: 1.67561603079236 arc0(1) + 1.01016678417558 arc0(2) + arc1(0)
       + 10 arc1(2) + 0.573476001924523 arc1(3) + arc2(0) + 10 arc2(1)
       + 3.48800564042764 arc2(3) + arc3(1) + arc3(2)
Subject To
 c1:  arc0(1) + arc0(2) + arc0(3) - arc1(0) - arc2(0) - arc3(0)  = 1
 c2:  arc0(1) - arc1(0) - arc1(2) - arc1(3) + arc2(1) + arc3(1)  = 0
 c3:  arc0(2) + arc1(2) - arc2(0) - arc2(1) - arc2(3) + arc3(2)  = 0
 c4:  arc0(3) + arc1(3) + arc2(3) + arc3(3)  = 1
 c5:  arc0(0) <= 0
 c6:  arc0(1) <= 0
 c7:  arc0(2) <= 8
 c8:  arc0(3) <= 0
 c9:  arc1(0) <= 8
 c10: arc1(1) <= 0
 c11: arc1(2) <= 10
 c12: arc1(3) <= 16
 c13: arc2(0) <= 8
 c14: arc2(1) <= 0
 c15: arc2(2) <= 0
 c16: arc2(3) <= 4
 c17: arc3(0) <= 0
 c18: arc3(1) <= 8
 c19: arc3(2) <= 8
 c20: arc3(3) <= 0
Bounds
 0 <= arc0(0) <= 1
 0 <= arc0(1) <= 1
 0 <= arc0(2) <= 1
 0 <= arc0(3) <= 1
 0 <= arc1(0) <= 1
 0 <= arc1(1) <= 1
 0 <= arc1(2) <= 1
 0 <= arc1(3) <= 1
 0 <= arc2(0) <= 1
 0 <= arc2(1) <= 1
 0 <= arc2(2) <= 1
 0 <= arc2(3) <= 1
 0 <= arc3(0) <= 1
 0 <= arc3(1) <= 1
 0 <= arc3(2) <= 1
 0 <= arc3(3) <= 1
Binaries
 arc0(0)  arc0(1)  arc0(2)  arc0(3)  arc1(0)  arc1(1)  arc1(2)  arc1(3) 
 arc2(0)  arc2(1)  arc2(2)  arc2(3)  arc3(0)  arc3(1)  arc3(2)  arc3(3) 
End
