

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}

	// you may add additional methods here
    void getFile(string name) {
        ofstream file(name);
        string condition = " ";
        while(condition != "done") {
            condition = this->read();
            if(condition == "done")
                break;
            file << condition << endl;
        }
    }
};

// you may add here helper classes


// you may edit this class
class Command{
protected:
	DefaultIO* dio;
public:
    string description;
    Command(DefaultIO* dio):dio(dio){}
	virtual void execute()=0;
	virtual ~Command(){}
};

// implement here your command classes
class UploadCommand : public Command {
public:
    UploadCommand(DefaultIO* dio):Command(dio) {
        description = "1.upload a time series csv file\n";
    }
    void execute() {
        dio->write("Please upload your local train CSV file.\n");
        dio->getFile("training.csv");
        dio->write("Upload complete.\n");

        dio->write("Please upload your local test CSV file.\n");
        dio->getFile("test.csv");
        dio->write("Upload complete.\n");
    }

    ~UploadCommand(){}
};

class AlgSettingsCommand : public Command {
    HybridAnomalyDetector* hybrid_detector;
public:
    AlgSettingsCommand(DefaultIO* dio, HybridAnomalyDetector* detector):Command(dio) {
        description = "2.algorithm settings\n";
        this->hybrid_detector = detector;
    }

    void execute() {
        dio->write("The current correlation threshold is " + to_string(this->hybrid_detector->getThreshold()) + "\n");
        float condition = 10;
        while(condition < 0 || condition > 1) {
            dio->write("Please choose a value between 0 and 1.\n");
            string input = dio->read();
            condition = stof(input);
        }
        this->hybrid_detector->setThreshold(condition);
    }

    ~AlgSettingsCommand(){}
};

class DetectAnomaliesCommand : public Command {
    HybridAnomalyDetector* hybrid_detector;
    vector<AnomalyReport>* ar;
public:
    DetectAnomaliesCommand(DefaultIO* dio, HybridAnomalyDetector* detector, vector<AnomalyReport>* ar):Command(dio) {
        description = "3.detect anomalies\n";
        this->hybrid_detector = detector;
        this->ar = ar;
    }

    void execute() {
        TimeSeries ts_train("training.csv");
        this->hybrid_detector->learnNormal(ts_train);
        TimeSeries ts_test("test.csv");
        *ar = this->hybrid_detector->detect(ts_test);
        dio->write("anomaly detection complete.\n");
    }

    ~DetectAnomaliesCommand(){}
};

class DisplayResultsCommand : public Command {
    vector<AnomalyReport>* ar;
public:
    DisplayResultsCommand(DefaultIO* dio, vector<AnomalyReport>* ar):Command(dio) {
        description = "4.display results\n";
        this->ar = ar;
    }

    void execute() {
        for(auto ptr = ar->begin(); ptr != ar->end(); ptr++) {
            dio->write(to_string(ptr->timeStep) + "\t" + ptr->description + "\n");
        }
        dio->write("Done.\n");
    }

    ~DisplayResultsCommand(){}
};

class UploadAndAnalyzeCommand : public Command {
    vector<AnomalyReport>* ar;
public:
    UploadAndAnalyzeCommand(DefaultIO* dio, vector<AnomalyReport>* ar):Command(dio) {
        description = "5.upload anomalies and analyze results\n";
        this->ar = ar;
    }

    void execute() {
        // receive local anomalies file
        vector<long> user_start, user_end;
        dio->write("Please upload your local anomalies file.\n");
        ofstream file("local_anomalies.txt");
        string line = " ";
        float sum = 0;
        while(line != "done") {
            line = dio->read();
            if(line == "done")
                break;
            auto index = line.find(',');
            int start = stoi(line.substr(0, index));
            int end = stoi(line.substr(index+1, line.length()));
            user_start.push_back(start);
            user_end.push_back(end);
            sum += end+1 - start;
        }
        dio->write("Upload complete.\n");
        // union
        vector<long> start, end;
        start.push_back(ar->at(0).timeStep);
        int size = ar->size();
        for(int i = 1; i < ar->size(); i++) {
            if(ar->at(i).description != ar->at(i-1).description) {
                end.push_back(ar->at(i-1).timeStep);
                start.push_back(ar->at(i).timeStep);
            }
        }
        end.push_back(ar->at(size-1).timeStep);
        // initialize P and N
        float P = user_start.size();
        TimeSeries ts("test.csv");
        float n = ts.getNumOfRows();
        float N = n - sum;
        // check
        float TP = 0, FP = 0;
        for (int i = 0; i < start.size(); i++) {
            bool check = false;
            for (int j = 0; j < P; j++) {
                if (!(start[i] > user_end[j] || user_start[j] > end[i])) {
                    TP++;
                    check = true;
                }
            }
            if (!check)
                FP++;
        }
        dio->write("True Positive Rate: " + round_to_three_digits(TP/P) + "\n");
        dio->write("False Positive Rate: " + round_to_three_digits(FP/N) + "\n");
    }

    string round_to_three_digits(float num) {
        string str = to_string(num);
        if (str.size() > 5) {
            str.erase(5, str.size());
            for (int i = 4; i >= 1; i--) {
                if (str[i] == '0' || str[i] == '.') {
                    str.erase(i);
                }
                else {
                    break;
                }
            }
        }
        return str;
    }

    ~UploadAndAnalyzeCommand(){}
};

class ExitCommand : public Command {
public:
    ExitCommand(DefaultIO* dio):Command(dio) {
        description = "6.exit\n";
    }

    void execute(){}

    ~ExitCommand(){}
};

#endif /* COMMANDS_H_ */
