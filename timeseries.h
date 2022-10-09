/*
 * timeseries.h
 *
 * Author: Nadav Vernia (ID 315535351) and Rotem Dgani (ID 208948927)
 */

#ifndef TIMESERIES_H_
#define TIMESERIES_H_
#include <iostream>
#include <string.h>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>


using namespace std;

class TimeSeries{
    vector<string> feats;
    map<string, vector<float>> data;
    int numOfRows;

public:

	TimeSeries(const char* CSVfileName){
        /*
         * this constructor reads a csv file given it's name, and save's it's data to a map containing a feature's name
         * and seen values, along with saving the feature's names to a different vector of strings
         */
        ifstream in(CSVfileName);
        string features;
        in>>features;
        istringstream ss(features);
        string feat;
        while (getline(ss, feat, ',')) {
            feats.push_back(feat);
            vector<float> values;
            data[feat] = values;
        }
        string lineStr;
        while (getline(in, lineStr)) {
            int i = 0;
            string value;
            istringstream line(lineStr);
            while (getline(line, value, ',')) {
                data[feats[i]].push_back(stof(value));
                i++;
            }
        }
        in.close();
        numOfRows = data[feats[0]].size();
	}

    size_t getNumOfRows() const {
        return numOfRows;
    }

    const vector<string>& getFeatures() const {
        // returns the feature's names
        return feats;
    }

    const vector<float>& getValue(string feat) const {
        // given a certain feature's name, it returns the vector of floats of the feature
        return data.at(feat);
    }
};



#endif /* TIMESERIES_H_ */
