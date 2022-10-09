/*
 * SimpleAnomalyDetector.h
 *
 * Author: Nadav Vernia (ID 315535351) and Rotem Dgani (ID 208948927)
 */

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

struct correlatedFeatures{
	string feature1,feature2;  // names of the correlated features
	float corrlation;
	Line lin_reg;
	float threshold, circlex, circley;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
protected:
    float threshold;
    vector<correlatedFeatures> cf;
public:
	SimpleAnomalyDetector();

    SimpleAnomalyDetector(float threshold);

    virtual ~SimpleAnomalyDetector();

	virtual vector<AnomalyReport> detect(const TimeSeries& ts);

	vector<correlatedFeatures> getNormalModel(){
		return cf;
	}

    void setThreshold(float threshold) {
        this->threshold = threshold;
    }

    float getThreshold() {
        return this->threshold;
    }

    float findMaxOffset(Line line, vector<float>& a, vector<float>& b) const;
    void learnNormal(const TimeSeries &ts);

protected:
    virtual bool check(float x, float y, int i);
    virtual void addToCF(vector<string> feats, int i, int j, float p, const TimeSeries &ts);
    virtual correlatedFeatures createCf(string a, string b, float pearson, const TimeSeries& ts) const;
};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
