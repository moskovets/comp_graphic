#ifndef F_H
#define F_H
#define FUNC_NUMBER 4
typedef double (*Pointfunc)(double, double);

class CFunction {

public:
    CFunction()
    {
        f[0] = &f1;
        f[1] = &f2;
        f[2] = &f3;
        f[3] = &f4;
    }

    Pointfunc GetFunc(int index)
    {
        return f[index];
    }

    const char* GetStr(int index)
    {
        switch(index) {
        case 0:
            return "y - sin(x)*cos(z)";
        case 1:
            return "y - x*x + z*z";
        case 2:
            return "y - sin(x)*x*cos(z)";
        case 3:
            return "y - x*z";
        }
        return "";
    }

private:
    Pointfunc f[FUNC_NUMBER];

    static double f1(double x, double z)
    {
        return sin(x) * cos(z);
    }
    static double f2(double x, double z)
    {
        return x*x - z*z;
    }
    static double f3(double x, double z)
    {
        return sin(x)*x*cos(z);
    }
    static double f4(double x, double z)
    {
        return x*z;
    }
};
#endif // F_H
