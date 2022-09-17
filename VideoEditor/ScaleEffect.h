#pragma once
#include "IEffect.h"
#include "EffectProperty.h"

class ScaleEffect :
    public IEffect
{
private:
    EffectProperty<double> scale;
public:
    void applyEffect(vector<Mat>&, SourceClip&, bool defult = false) override;
};

vector <float> sigmoid(const vector <float>& m1) {

    /*  Returns the value of the sigmoid function f(x) = 1/(1 + e^-x).
        Input: m1, a vector.
        Output: 1/(1 + e^-x) for every element of the input matrix m1.
    */
    const unsigned long VECTOR_SIZE = m1.size();
    vector <float> output(VECTOR_SIZE);


    for (unsigned i = 0; i != VECTOR_SIZE; ++i) {
        output[i] = 1 / (1 + exp(-m1[i]));
    }

    return output;
}