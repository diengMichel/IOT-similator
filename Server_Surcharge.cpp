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
    // Constructeur par défaut
    Server(int nbrOfSensors = 0, bool consoleActivation = true, bool logActivation = true)
        : nbrOfSensors(nbrOfSensors), consoleActivation(consoleActivation), logActivation(logActivation) {}

    // Destructeur
    ~Server() {}

    // Fonction pour recevoir les données des capteurs
    void dataRcv(int dataSens) {
        if (consoleActivation) {
            consolWrite(dataSens);
        }

        if (logActivation) {
            fileWrite(dataSens);
        }
    }

    // Fonction pour écrire dans la console
    void consolWrite(int dataSens) {
        std::cout << "Données reçues dans la console : " << dataSens << std::endl;
    }

    // Fonction pour écrire dans un fichier de logs
    void fileWrite(int dataSens) {
        std::ofstream logfile("sensor_log.txt", std::ios::app);
        if (logfile.is_open()) {
            logfile << "Données du capteur : " << dataSens << std::endl;
            logfile.close();
        } else {
            std::cerr << "Erreur : Impossible d'ouvrir le fichier de logs" << std::endl;
        }
    }

    // Surcharge de l'opérateur '='
    Server& operator=(const Server& other) {
        if (this == &other) {
            return *this; // Éviter l'auto-attribution
        }

        nbrOfSensors = other.nbrOfSensors;
        consoleActivation = other.consoleActivation;
        logActivation = other.logActivation;

        return *this;
    }

    // Surcharge de l'opérateur '<<' pour afficher dans la console
    friend std::ostream& operator<<(std::ostream& os, const Server& server) {
        os << "Nombre de capteurs : " << server.nbrOfSensors << std::endl;
        os << "Activation de la console : " << (server.consoleActivation ? "Activée" : "Désactivée") << std::endl;
        os << "Activation du log : " << (server.logActivation ? "Activée" : "Désactivée") << std::endl;
        return os;
    }

    // Surcharge de l'opérateur '<<' pour écrire dans un fichier de log
    friend void operator<<(const Server& server, const std::string& filename) {
        std::ofstream logfile(filename, std::ios::app);
        if (logfile.is_open()) {
            logfile << "Nombre de capteurs : " << server.nbrOfSensors << std::endl;
            logfile << "Activation de la console : " << (server.consoleActivation ? "Activée" : "Désactivée") << std::endl;
            logfile << "Activation du log : " << (server.logActivation ? "Activée" : "Désactivée") << std::endl;
            logfile.close();
        } else {
            std::cerr << "Erreur : Impossible d'ouvrir le fichier de logs" << std::endl;
        }
    }
};

int main() {
    SetConsoleOutputCP(CP_UTF8);
    
    Server server1(5, true, true);
    Server server2;

    // Copier le contenu de server1 dans server2
    server2 = server1;

    // Afficher les informations dans la console
    std::cout << server2;

    // Écrire les informations dans un fichier de log
    server2 << "server_log.txt";

    return 0;
}