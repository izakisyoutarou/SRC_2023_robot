#include "utilplus.hpp"
#include <cmath>

namespace rp_lib {

int quadraticEquationSolver(float &x1_arg,float &x1i_arg,float &x2_arg,float &x2i_arg
                            ,float a,float b,float c){//a*x^2+b*x^1+c=0
    std::complex<float> x1,x2;

    //calc
    typedef std::complex<float> compf;
    x1=(-b+ std::sqrt(compf(b*b-4*a*c,0.0f)) )/(2.0f*a);
    x2=(-b- std::sqrt(compf(b*b-4*a*c,0.0f)) )/(2.0f*a);

    //assignment
    x1_arg=x1.real();
    x1i_arg=x1.imag();
    x2_arg=x2.real();
    x2i_arg=x2.imag();

    //return number of complex roots
    if(x1i_arg!=0.0&&x2i_arg!=0.0)return 2;
    else return 0;
}

int quadraticEquationSolver(std::complex<float> &x1,std::complex<float> &x2
                            ,float a,float b,float c){//a*x^2+b*x^1+c=0
    //calc
    typedef std::complex<float> compf;
    x1=(-b+ std::sqrt(compf(b*b-4*a*c,0.0f)) )/(2.0f*a);
    x2=(-b- std::sqrt(compf(b*b-4*a*c,0.0f)) )/(2.0f*a);

    //return number of complex roots
    if(x1.imag()!=0.0f&&x2.imag()!=0.0f)return 2;
    else return 0;
}


int cubicEquationSolver(std::complex<float> &x1,std::complex<float> &x2,std::complex<float> &x3
                        ,float a,float b,float c,float d){
    typedef std::complex<float> compf;
    //a*x^3+c*x^2+c*x^1+d=0

    //x^3+A*x^2+B*x+C=0
    float A=b/a;
    float B=c/a;
    float C=d/a;

    //X=x+A/3
    //assign x=X-A/3
    //X^3+p*X+q=0
    //X^3 + (B - A^2 /3)*X + (2*A^3)/27 - (B*A)/3 + C = 0
    float p=(B-A*A/3.0f);
    float q=(2.0f*A*A*A)/27.0f-B*A/3.0f+C;

    compf w=compf(-1.0f/2.0f,std::sqrt(3.0f)/2.0f);

    compf uCube=-q/2.0f+std::sqrt(compf(q*q/4.0f+p*p*p/27.0f,0.0f));
    compf u=std::pow(uCube,1.0f/3.0f);

    compf u1=u;
    compf u2=u*w;
    compf u3=u*w*w;

    compf v1,v2,v3;

    //Is assign 0 correct?
    if(u1==compf(0.0f,0.0f))v1=compf(0.0f,0.0f);
    else v1=-p/(3.0f*u1);
    if(u2==compf(0.0f,0.0f))v2=compf(0.0f,0.0f);
    else v2=-p/(3.0f*u2);
    if(u3==compf(0.0f,0.0f))v3=compf(0.0f,0.0f);
    else v3=-p/(3.0f*u3);

    compf X1=u1+v1;
    compf X2=u2+v2;
    compf X3=u3+v3;

    x1=X1-A/3.0f;
    x2=X2-A/3.0f;
    x3=X3-A/3.0f;

    int i=0;//return number of complex roots
    if(x1.imag()!=0.0f) i++;
    if(x2.imag()!=0.0f) i++;
    if(x3.imag()!=0.0f) i++;

    return i;
}

int cubicEquationSolver(float &x1r,float &x1i,float &x2r,float &x2i,float &x3r,float &x3i
                        ,float a,float b,float c,float d){
    typedef std::complex<float> compf;
    compf x1,x2,x3;

    int i=cubicEquationSolver(x1,x2,x3,a,b,c,d);

    x1r=x1.real();
    x2r=x2.real();
    x3r=x3.real();
    x1i=x1.imag();
    x2i=x2.imag();
    x3i=x3.imag();

    return i;
}

int combination(int n, int r) {
    if (r == 0 || r == n)
        return (1);
    else if (r == 1)
        return (n);
    return (combination(n - 1, r - 1) + combination(n - 1, r));
}

}