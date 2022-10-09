

#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"

using namespace std;

class CLI {
	DefaultIO* dio;
    HybridAnomalyDetector detector;
    vector<AnomalyReport> ar;
    vector<Command*> commands;
	// you can add data members
public:
	CLI(DefaultIO* dio);
	void start();
	virtual ~CLI();
    void print_menu();
    };

#endif /* CLI_H_ */
