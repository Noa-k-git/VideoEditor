#include <vector>;
#include <cmath>;

using std::vector;

#pragma once
class Transition
{

    static vector<float> linear(int);
	void sigmoid(vector <float>& m); /*  Returns the value of the sigmoid function f(x) = 1/(1 + e^-x).
       Input: m, a vector.
       Output: 1/(1 + e^-x) for every element of the input matrix m.
   */
    void logTen(vector<float>& m);
    void logE(vector<float>& m);


};

