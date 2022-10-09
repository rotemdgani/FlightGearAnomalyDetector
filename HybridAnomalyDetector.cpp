
#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {
	// TODO Auto-generated constructor stub

}

HybridAnomalyDetector::~HybridAnomalyDetector() {
	// TODO Auto-generated destructor stub
}

correlatedFeatures HybridAnomalyDetector::createCf(string a, string b, float pearson, const TimeSeries& ts) const{
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
    Circle c = findMinCircle(p, size);
    cor_f.circlex = c.center.x;
    cor_f.circley = c.center.y;

    cor_f.threshold = c.radius * 1.1;
    for (int i = 0; i < size; i++) {
        delete p[i];
    }
    delete[] p;
    return cor_f;
}

//void HybridAnomalyDetector::learnNormal(const TimeSeries& ts) {
//    SimpleAnomalyDetector::learnNormal(ts);
//}

void HybridAnomalyDetector::addToCF(vector<string> feats, int i, int j, float p, const TimeSeries& ts){
    SimpleAnomalyDetector::addToCF(feats, i, j, p, ts);
    if (j != -1 && p > 0.5 && p < threshold) {
        cf.push_back(HybridAnomalyDetector::createCf(feats[i], feats[j], p, ts));
    }
}

bool HybridAnomalyDetector::check(float x, float y, int i) {
    Point a = Point(cf[i].circlex, cf[i].circley);
    Point b = Point(x, y);
    bool flag = (cf[i].corrlation >= threshold && SimpleAnomalyDetector::check(x, y, i)) ||
    (cf[i].corrlation < threshold && cf[i].corrlation > 0.5 && twoPointsDistance(a, b) > cf[i].threshold);
    return flag;
}