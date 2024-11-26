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

    template<typename T>
    void ReadMemoryArray(DWORD address, T* buffer, size_t count) {
        SIZE_T bytesRead = 0;

        // Считаем весь массив
        if (!ReadProcessMemory(processHandle, reinterpret_cast<LPCVOID>(address), buffer, sizeof(T) * count, &bytesRead)) {
            throw std::runtime_error("Ошибка чтения памяти массива. Код ошибки: " + std::to_string(GetLastError()));
        }

        // Проверяем, что было прочитано нужное количество байт
        if (bytesRead != sizeof(T) * count) {
            throw std::runtime_error("Прочитано недостаточно данных из памяти.");
        }
    }
};

#endif // PROCESS_MEMORY_H
