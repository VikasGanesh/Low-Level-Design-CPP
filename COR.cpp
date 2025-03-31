#include <iostream>
#include <memory>
#include <string>

enum LogLevel {
    INFO,
    DEBUG,
    ERROR
};

class Logger {
protected:
    std::shared_ptr<Logger> nextLogger;

public:
    void setNextLogger(std::shared_ptr<Logger> nextLogger) {
        this->nextLogger = nextLogger;
    }

    void log(LogLevel level, const std::string& message) {
        if (this->logMessage(level, message)) {
            return;
        }
        if (nextLogger != nullptr) {
            nextLogger->log(level, message);
        }
    }

    virtual ~Logger() {}

protected:
    virtual bool logMessage(LogLevel level, const std::string& message) = 0;
};
class InfoLogger : public Logger {
protected:
    bool logMessage(LogLevel level, const std::string& message) override {
        if (level == INFO) {
            std::cout << "INFO: " << message << std::endl;
            return true;
        }
        return false;
    }
};

class DebugLogger : public Logger {
protected:
    bool logMessage(LogLevel level, const std::string& message) override {
        if (level == DEBUG) {
            std::cout << "DEBUG: " << message << std::endl;
            return true;
        }
        return false;
    }
};

class ErrorLogger : public Logger {
protected:
    bool logMessage(LogLevel level, const std::string& message) override {
        if (level == ERROR) {
            std::cout << "ERROR: " << message << std::endl;
            return true;
        }
        return false;
    }
};
int main() {
    // Create loggers
    std::shared_ptr<Logger> errorLogger = std::make_shared<ErrorLogger>();
    std::shared_ptr<Logger> debugLogger = std::make_shared<DebugLogger>();
    std::shared_ptr<Logger> infoLogger = std::make_shared<InfoLogger>();

    // Set up the chain
    infoLogger->setNextLogger(debugLogger);
    debugLogger->setNextLogger(errorLogger);

    // Test the chain
    infoLogger->log(INFO, "This is an information message.");
    infoLogger->log(DEBUG, "This is a debug message.");
    infoLogger->log(ERROR, "This is an error message.");

    return 0;
}
