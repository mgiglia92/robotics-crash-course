
class Differentiator{

public:
    //Constructors
    Differentiator(double, double);

    //members
    double sigma; //dirty derivative bandwidth = 1/sigma
    double Ts; //sample period in sec
    double beta; //(2.0*sigma-Ts)/(2.0*sigma+Ts)


    //methods
    double differentiate(double);
    void update_time_parameters(double, double);
    void reset(double);

private:
    double y_d1; //differentiation variable 1 sample behind
    double y_dot; //derivative of y
};