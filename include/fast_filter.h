#ifndef FAST_FILTER_H
#define FAST_FILTER_H

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

    class MovingAverageFilter {
    public:
        MovingAverageFilter(int windowSize);
        int add(int newValue);
        int getFilteredValue();
        void reset();

    private:
        int* _values;
        int _windowSize;
        long _sum;
        int _count;
        int _index;
    };

    int applyExponentialFilter(int currentValue, int previousValue, float alpha);

#ifdef __cplusplus
}
#endif

#endif // FAST_FILTER_H