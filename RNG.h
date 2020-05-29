#pragma once
#include<cmath> 
 
inline double getNormal(double Mx, double Sigma)
{
    double  a, b, r, Sq;
    do
    {
        a = (double)(rand()) / RAND_MAX;
        b = (double)(rand()) / RAND_MAX;
        r = a * a + b * b;
    } while (r >= 1);
    Sq = sqrt(-2.0 * log(r) / r);
    return Mx + Sigma * a * Sq;
}

inline double getExp(double m)
{
    double a;
    a = (double)(rand()) / RAND_MAX;
    return -log(a) * (m);
}






	
	
