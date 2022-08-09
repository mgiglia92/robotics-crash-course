
class Differentiator{

public:
    //Constructors
    Differentiator(float, float);

    //members
    float sigma; //dirty derivative bandwidth = 1/sigma
    float ts; //sample period in sec
    float beta; //(2.0*sigma-ts)/(2.0*sigma+ts)


    //methods
    float differentiate(float);
    void update_time_parameters(float, float);
    void reset(float);

private:
    float y_d1; //differentiation variable 1 sample behind
    float y_dot; //derivative of y
};