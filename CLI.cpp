#include "CLI.h"

CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
    detector = HybridAnomalyDetector();
    ar = vector<AnomalyReport>();
    commands.push_back(new UploadCommand(dio));
    commands.push_back(new AlgSettingsCommand(dio, &detector));
    commands.push_back(new DetectAnomaliesCommand(dio, &detector, &ar));
    commands.push_back(new DisplayResultsCommand(dio, &ar));
    commands.push_back(new UploadAndAnalyzeCommand(dio, &ar));
    commands.push_back(new ExitCommand(dio));
}

void CLI::print_menu() {
    dio->write("Welcome to the Anomaly Detection Server.\n");
    dio->write("Please choose an option:\n");
    for (int i = 0; i < commands.size(); i++) {
        dio->write(commands[i]->description);
    }
}

void CLI::start(){
    // vector<Command*> commands = create_commands();
    int input = 0;
    while (input != 6) {
        print_menu();
        input = stoi(dio->read());
        if (input < 1 || input > 6)
            continue;
        commands[input-1]->execute();
    }
}


CLI::~CLI() {
    for (int i = 0; i < commands.size(); i++) {
        delete commands[i];
    }
}

