#include <iostream>
#include <random>
#include <windows.h>

// Classe de base Sensor
class Sensor {
protected:
    int valSense;

public:
    // Constructeur par défaut
    Sensor() : valSense(0) {}

    // Constructeur par copie
    Sensor(const Sensor& other) : valSense(other.valSense) {}

    // Constructeur avec initialisation de la valeur
    Sensor(int valRcv) : valSense(valRcv) {}

    // Destructeur
    virtual ~Sensor() {}

    // Méthode pour envoyer la valeur du capteur
    virtual int sendData() {
        return valSense;
    }

    // Méthode virtuelle pure pour générer les données
    virtual void generateData() = 0;

protected:
    // Méthode pour générer une valeur aléatoire (simulation)
    int aleaGenVal() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distribution(0, 100);
        return distribution(gen);
    }
};

// Classe dérivée Temperature
class Temperature : public Sensor {
public:
    // Constructeur par défaut
    Temperature() {}

    // Constructeur avec initialisation de la valeur
    Temperature(float valRcv) : Sensor(static_cast<int>(valRcv * 10)) {}

    // Méthode pour générer des données de température (FLOAT)
    void generateData() override {
        // Génération de données de température (ex. entre 0 et 100 degrés Celsius)
        valSense = aleaGenVal();
    }
};

// Classe dérivée Humidity
class Humidity : public Sensor {
public:
    // Constructeur par défaut
    Humidity() {}

    // Constructeur avec initialisation de la valeur
    Humidity(float valRcv) : Sensor(static_cast<int>(valRcv * 10)) {}

    // Méthode pour générer des données d'humidité (FLOAT)
    void generateData() override {
        // Génération de données d'humidité (ex. entre 0% et 100%)
        valSense = aleaGenVal();
    }
};

// Classe dérivée Light
class Light : public Sensor {
public:
    // Constructeur par défaut
    Light() {}

    // Constructeur avec initialisation de la valeur
    Light(bool valRcv) : Sensor(valRcv ? 1 : 0) {}

    // Méthode pour générer des données de lumière (BOOLEAN)
    void generateData() override {
        // Génération de données de lumière (true ou false)
        valSense = aleaGenVal() > 50 ? 1 : 0;
    }
};

// Classe dérivée Sound
class Sound : public Sensor {
public:
    // Constructeur par défaut
    Sound() {}

    // Constructeur avec initialisation de la valeur
    Sound(int valRcv) : Sensor(valRcv) {}

    // Méthode pour générer des données de son (INTEGER)
    void generateData() override {
        // Génération de données de son (ex. entre 0 et 100 décibels)
        valSense = aleaGenVal();
    }
};

int main() {
    SetConsoleOutputCP(CP_UTF8);
    // Exemple d'utilisation des capteurs
    Temperature tempSensor;
    Humidity humiditySensor;
    Light lightSensor;
    Sound soundSensor;

    // Générer des données pour chaque capteur
    tempSensor.generateData();
    humiditySensor.generateData();
    lightSensor.generateData();
    soundSensor.generateData();

    // Afficher les données générées par les capteurs
    std::cout << "Température : " << static_cast<float>(tempSensor.sendData()) / 10.0 << " °Celsius" << std::endl;
    std::cout << "Humidité : " << static_cast<float>(humiditySensor.sendData()) / 10.0 << "%" << std::endl;
    std::cout << "Lumière : " << (lightSensor.sendData() ? "On" : "Off") << std::endl;
    std::cout << "Son : " << soundSensor.sendData() << " dB" << std::endl;

    return 0;
}