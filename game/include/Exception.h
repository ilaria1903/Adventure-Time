#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>

class GameException : public std::exception {
public:
    explicit GameException(const std::string& message) : message(message) {}
    virtual const char* what() const noexcept override {
        return message.c_str();
    }

private:
    std::string message;
};

class FileLoadException : public GameException {
public:
    explicit FileLoadException(const std::string& message) : GameException("File Load Error: " + message) {}
};

class InvalidOperationException : public GameException {
public:
    explicit InvalidOperationException(const std::string& message) : GameException("Invalid Operation: " + message) {}
};

class PlayerOutOfBoundsException : public GameException {
public:
    explicit PlayerOutOfBoundsException(const std::string& message) : GameException("Player Out of Bounds: " + message) {}
};

#endif // EXCEPTION_H