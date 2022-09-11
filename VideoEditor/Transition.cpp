#include "Transition.h"

vector<float> Transition::linear(int frames)
{
    vector<float> m;
    float size = 1 / (frames - 1);
    m.push_back(0);
    for (int i = 0; i < frames; i++)
    {
        m.push_back(size * i);
    }
    return m;
}

void Transition::sigmoid(vector<float>& m)
{
    for (unsigned i = 1; i < m.size()-1; i++) {
        double x = m[i];
        m[i] = 1 / (1 + exp(-10*(x-0.5)));
    }
}

void Transition::logTen(vector<float>& m)
{
    for (unsigned i = 1; i < m.size() - 1; i++) {
        double x = m[i];
        m[i] = 0.5 + log10(x/(1-x))/6;
    }
}

void Transition::logE(vector<float>& m)
{
    for (unsigned i = 1; i < m.size()-1; i++) {
        double x = m[i];
        m[i] = log(1 + exp(5.0414 * x - 4.5));
    }
}

