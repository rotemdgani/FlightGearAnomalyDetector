/*
 * SimpleAnomalyDetector.cpp
 *
 * Author: Nadav Vernia (ID 315535351) and Rotem Dgani (ID 208948927)
 */

#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
    this->threshold = 0.9;
}

SimpleAnomalyDetector::SimpleAnomalyDetector(float threshold) {
    this->threshold = threshold;
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
    // TODO Auto-generated destructor stub
}

correlatedFeatures SimpleAnomalyDetector::createCf(string a, string b, float pearson, const TimeSeries& ts) const {
    /*
     * this method creates and returns a new correlatedFeatures struct using the given parameters and other
     * calculations, like creating a pointer to a Point pointers array containing the values of the two correlated
     * features, and afterwards sending that pointer to the linear_reg function to calculate it.
     */
    correlatedFeatures cor_f;
    cor_f.feature1 = a;
    cor_f.feature2 = b;
    cor_f.corrlation = pearson;

    int size = ts.getNumOfRows();
    vector<float> d1 = ts.getValue(a);
    vector<float> d2 = ts.getValue(b);
    Point** p = new Point*[size];
    for (int i = 0; i < size; i++) {
        p[i] = new Point(d1[i], d2[i]);
    }
    cor_f.lin_reg = linear_reg(p, size);
    cor_f.threshold = findMaxOffset(cor_f.lin_reg, d1, d2);
    for (int i = 0; i < size; i++) {
        delete p[i];
    }
    delete[] p;
    return cor_f;
}

float SimpleAnomalyDetector::findMaxOffset(Line line, vector<float>& a, vector<float>& b) const {
    /*
     * this method goes over all correlated features and finds the max offset from the regression line, and returns
     * that value multiplied by 1.1
     */
    float cf_threshold = 0.0;
    int size = a.size();
    for (int i = 0; i < size; i++) {
        float delta = abs(b[i] - line.f(a[i]));
        if (delta > cf_threshold) {
            cf_threshold = delta;
        }
    }
    return cf_threshold * 1.1;
}

void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
    /*
     * this method goes over all possible combinations of features and checks if they are correlated by having their
     * pearson correlation value above the threshold, which is 0.9. eventually, for every feature, it saves the most
     * correlated feature to it (if there is one), and adds it to the vector of correlated features.
     */
	int n = ts.getNumOfRows();
    vector<string> features = ts.getFeatures();
    for (int i = 0; i < n; i++) {
        float m = 0.0;
        int c = -1;
        for (int j = i+1; j < features.size(); j++) {
            float x[n], y[n];
            vector<float> f1 = ts.getValue(features[i]);
            vector<float> f2 = ts.getValue(features[j]);
            for (int k = 0; k < n; k++) {
                x[k] = f1[k];
                y[k] = f2[k];
            }
            float p = abs(pearson(x, y, n));
            if (p > m) {
                m = p;
                c = j;
            }
        }

        addToCF(features, i, c, m, ts);
    }
}

void SimpleAnomalyDetector::addToCF(vector<string> feats, int i, int j, float p, const TimeSeries& ts){
    if (j != -1 && p > threshold) {
        cf.push_back(SimpleAnomalyDetector::createCf(feats[i], feats[j], p, ts));
    }
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
    /*
     * this method detects anomalies by going over the features that are correlated in the unseen data that wasn't
     * used when running the learnNormal method, and if there are instances where the two values go over the threshold
     * calculated beforehand in findMaxOffset, it "marks" them as anomalies and adds them to the AnomalyReport vector.
     */
    vector<AnomalyReport> ar;
	for (int i = 0; i < cf.size(); i++) {
        vector<float> f1 = ts.getValue(cf[i].feature1);
        vector<float> f2 = ts.getValue(cf[i].feature2);
        for (int j = 0; j < f1.size(); j++) {
            if (check(f1[j], f2[j], i)) {
                string description = cf[i].feature1 + "-" + cf[i].feature2;
                ar.emplace_back(description, j+1);
            }
        }
    }
    return ar;
}

bool SimpleAnomalyDetector::check(float x, float y, int i) {
    return abs(cf[i].lin_reg.f(x) - y) > cf[i].threshold;
}