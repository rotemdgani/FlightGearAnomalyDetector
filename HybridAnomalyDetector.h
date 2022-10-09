

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();
protected:
    void addToCF(vector<string> feats, int i, int j, float p, const TimeSeries &ts);

    correlatedFeatures createCf(string a, string b, float pearson, const TimeSeries &ts) const;

    bool check(float x, float y, int i);

    //void learnNormal(const TimeSeries &ts);
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
