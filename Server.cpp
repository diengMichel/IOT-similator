#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

class Server {
private:
    int nbrOfSensors;
    bool consoleActivation;
    bool logActivation;

public:
    Server(int nbrOfSensors = 0, bool consoleActivation = true, bool logActivation = true)
        : nbrOfSensors(nbrOfSensors), consoleActivation(consoleActivation), logActivation(logActivation) {}

    ~Server() {}

    void dataRcv(int dataSens, const std::string& filename) {
        if (consoleActivation) {
            consolWrite(dataSens);
        }

        if (logActivation) {
            fileWrite(dataSens, filename);
        }
    }

    void consolWrite(int dataSens) {
        std::cout << "Données reçues dans la console : " << dataSens << std::endl;
    }

    void fileWrite(int dataSens, const std::string& filename) {
        std::ofstream logfile(filename, std::ios::app);
        if (logfile.is_open()) {
            logfile << "Données du capteur : " << dataSens << std::endl;
            logfile.close();
        } else {
            std::cerr << "Erreur : Impossible d'ouvrir le fichier de logs " << filename << std::endl;
        }
    }
};

int main() {
    SetConsoleOutputCP(CP_UTF8);
    Server server;

    server.dataRcv(42, "sensor1_log.txt");
    server.dataRcv(67, "sensor2_log.txt");

    return 0;
}
