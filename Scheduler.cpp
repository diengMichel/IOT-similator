#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <random>
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

    void dataRcv(const std::string& sensorName, const std::string& data, const std::string& unit, const std::string& fileName) {
        if (consoleActivation) {
            consolWrite(sensorName, data, unit);
        }

        if (logActivation) {
            fileWrite(sensorName, data, unit, fileName);
        }
    }

    void consolWrite(const std::string& sensorName, const std::string& data, const std::string& unit) {
        std::cout << sensorName << " : " << data << " " << unit << std::endl;
    }

    void fileWrite(const std::string& sensorName, const std::string& data, const std::string& unit, const std::string& fileName) {
        std::ofstream logfile(fileName, std::ios::app);
        if (logfile.is_open()) {
            logfile << sensorName << " : " << data << " " << unit << std::endl;
            logfile.close();
        } else {
            std::cerr << "Error: Unable to open the log file " << fileName << std::endl;
        }
    }

    Server& operator=(const Server& other) {
        if (this == &other) {
            return *this;
        }

        nbrOfSensors = other.nbrOfSensors;
        consoleActivation = other.consoleActivation;
        logActivation = other.logActivation;

        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const Server& server) {
        os << "Number of sensors: " << server.nbrOfSensors << std::endl;
        os << "Console activation: " << (server.consoleActivation ? "Enabled" : "Disabled") << std::endl;
        os << "Log activation: " << (server.logActivation ? "Enabled" : "Disabled") << std::endl;
        return os;
    }
};

class Sensor {
protected:
    int valSense;
    std::string sensorName;
    std::string unit;

public:
    Sensor(const std::string& name, const std::string& unit) : valSense(0), sensorName(name), unit(unit) {}

    virtual ~Sensor() {}

    virtual int sendData() {
        return valSense;
    }

    virtual void generateData() = 0;

    std::string name() const {
        return sensorName;
    }

    std::string getUnit() const {
        return unit;
    }

    int aleaGenVal() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distribution(0, 100);
        return distribution(gen);
    }
};

class Temperature : public Sensor {
public:
    Temperature(const std::string& name, const std::string& unit) : Sensor(name, unit) {}

    void generateData() override {
        valSense = aleaGenVal();
    }
};

class Humidity : public Sensor {
public:
    Humidity(const std::string& name, const std::string& unit) : Sensor(name, unit) {}

    void generateData() override {
        valSense = aleaGenVal();
    }
};

class Light : public Sensor {
public:
    Light(const std::string& name, const std::string& unit) : Sensor(name, unit) {}

    void generateData() override {
        valSense = aleaGenVal() > 50 ? 1 : 0;
    }
};

class Sound : public Sensor {
public:
    Sound(const std::string& name, const std::string& unit) : Sensor(name, unit) {}

    void generateData() override {
        valSense = aleaGenVal();
    }
};

class Scheduler {
private:
    Server& server;
    std::vector<Sensor*> sensors;
    std::vector<std::chrono::milliseconds> intervals;
    std::vector<std::string> fileNames;

public:
    Scheduler(Server& srv) : server(srv) {}

    void addSensor(Sensor* sensor, std::chrono::milliseconds interval, const std::string& fileName) {
        sensors.push_back(sensor);
        intervals.push_back(interval);
        fileNames.push_back(fileName);
    }

    void start() {
        for (size_t i = 0; i < sensors.size(); ++i) {
            std::thread([this, i]() {
                while (true) {
                    std::this_thread::sleep_for(intervals[i]);
                    sensors[i]->generateData();
                    int data = sensors[i]->sendData();
                    server.dataRcv(sensors[i]->name(), std::to_string(data), sensors[i]->getUnit(), fileNames[i]);
                }
            }).detach();
        }

        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
};

int main() {
    SetConsoleOutputCP(CP_UTF8);
    Server server;

    Temperature temperatureSensor("Température", "°C");
    Humidity humiditySensor("Humidité", "%");
    Light lightSensor("Lumière", "lux");
    Sound soundSensor("Son", "dB");

    Scheduler scheduler(server);
    scheduler.addSensor(&temperatureSensor, std::chrono::milliseconds(1000), "temperature_log.txt");
    scheduler.addSensor(&humiditySensor, std::chrono::milliseconds(1500), "humidity_log.txt");
    scheduler.addSensor(&lightSensor, std::chrono::milliseconds(2000), "light_log.txt");
    scheduler.addSensor(&soundSensor, std::chrono::milliseconds(3000), "sound_log.txt");

    scheduler.start();

    return 0;
}
