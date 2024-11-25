#ifndef PROCESS_MEMORY_H
#define PROCESS_MEMORY_H

#include <windows.h>
#include <stdexcept>
#include <string>

class ProcessMemory {
private:
    HANDLE processHandle; // Дескриптор процесса

public:
    // Конструктор: принимает дескриптор процесса
    explicit ProcessMemory(HANDLE handle) : processHandle(handle) {
        if (processHandle == nullptr || processHandle == INVALID_HANDLE_VALUE) {
            throw std::runtime_error("Недействительный дескриптор процесса");
        }
    }

    // Деструктор: автоматически закрывает дескриптор процесса
    ~ProcessMemory() {
        if (processHandle) {
            CloseHandle(processHandle);
        }
    }

    // Метод для чтения памяти по DWORD-адресу
    template<typename T>
    T ReadMemory(DWORD address) {
        T buffer{};
        SIZE_T bytesRead = 0;

        if (!ReadProcessMemory(processHandle, reinterpret_cast<LPCVOID>(address), &buffer, sizeof(T), &bytesRead)) {
            throw std::runtime_error("Ошибка чтения памяти. Код ошибки: " + std::to_string(GetLastError()));
        }

        return buffer;
    }

    // Метод для записи в память по DWORD-адресу
    template<typename T>
    void WriteMemory(DWORD address, const T& value) {
        SIZE_T bytesWritten = 0;

        if (!WriteProcessMemory(processHandle, reinterpret_cast<LPVOID>(address), &value, sizeof(T), &bytesWritten)) {
            throw std::runtime_error("Ошибка записи в память. Код ошибки: " + std::to_string(GetLastError()));
        }
    }
};

#endif // PROCESS_MEMORY_H
