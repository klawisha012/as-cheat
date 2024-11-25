#ifndef PROCESS_MEMORY_H
#define PROCESS_MEMORY_H

#include <windows.h>
#include <stdexcept>
#include <string>

class ProcessMemory {
private:
    HANDLE processHandle; // ���������� ��������

public:
    // �����������: ��������� ���������� ��������
    explicit ProcessMemory(HANDLE handle) : processHandle(handle) {
        if (processHandle == nullptr || processHandle == INVALID_HANDLE_VALUE) {
            throw std::runtime_error("���������������� ���������� ��������");
        }
    }

    // ����������: ������������� ��������� ���������� ��������
    ~ProcessMemory() {
        if (processHandle) {
            CloseHandle(processHandle);
        }
    }

    // ����� ��� ������ ������ �� DWORD-������
    template<typename T>
    T ReadMemory(DWORD address) {
        T buffer{};
        SIZE_T bytesRead = 0;

        if (!ReadProcessMemory(processHandle, reinterpret_cast<LPCVOID>(address), &buffer, sizeof(T), &bytesRead)) {
            throw std::runtime_error("������ ������ ������. ��� ������: " + std::to_string(GetLastError()));
        }

        return buffer;
    }

    // ����� ��� ������ � ������ �� DWORD-������
    template<typename T>
    void WriteMemory(DWORD address, const T& value) {
        SIZE_T bytesWritten = 0;

        if (!WriteProcessMemory(processHandle, reinterpret_cast<LPVOID>(address), &value, sizeof(T), &bytesWritten)) {
            throw std::runtime_error("������ ������ � ������. ��� ������: " + std::to_string(GetLastError()));
        }
    }
};

#endif // PROCESS_MEMORY_H
