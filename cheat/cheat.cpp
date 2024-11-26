#include <iostream>
#include "offsets.h"
#include "ProcessMemory.h"
#include <chrono>
#include <thread>


using namespace std;
using namespace chrono;

struct Vec3 {
	float x, y, z;
};

struct Vec2 {
	float x, y;
};

bool WorldToScreen(const Vec3& worldPos, Vec2& screenPos, float* viewMatrix, int screenWidth, int screenHeight) {
	// Вычисляем значения W
	float w = viewMatrix[12] * worldPos.x + viewMatrix[13] * worldPos.y + viewMatrix[14] * worldPos.z + viewMatrix[15];

	// Проверяем видимость
	if (w < 0.01f) {
		return false; // Объект за пределами экрана
	}

	// Преобразование координат
	screenPos.x = (viewMatrix[0] * worldPos.x + viewMatrix[1] * worldPos.y + viewMatrix[2] * worldPos.z + viewMatrix[3]) / w;
	screenPos.y = (viewMatrix[4] * worldPos.x + viewMatrix[5] * worldPos.y + viewMatrix[6] * worldPos.z + viewMatrix[7]) / w;

	// Приводим к экранным координатам
	screenPos.x = (screenWidth / 2.0f) * (1.0f + screenPos.x);
	screenPos.y = (screenHeight / 2.0f) * (1.0f - screenPos.y); // Инвертируем Y, т.к. экранная система координат имеет верхний левый угол (0,0)

	return true; // Объект видим
}

void monitorPlayers(ProcessMemory& pm, DWORD playerCount, DWORD entityList) {
	while (true) {
		// Получаем количество игроков
		playerCount = pm.ReadMemory<DWORD>(offsets::PlayerCount);
		std::cout << "Количество игроков: " << playerCount << std::endl;

		// Считываем данные каждого игрока
		for (DWORD i = 0; i < playerCount; i++) {
			DWORD entityAddress = pm.ReadMemory<DWORD>(entityList + i * 0x4); // Каждый игрок занимает 0x4
			if (!entityAddress) {
				continue; // Если сущность не существует, пропускаем
			}

			// Чтение координат игрока
			float posX = pm.ReadMemory<float>(entityAddress + offsets::PositionX);
			float posY = pm.ReadMemory<float>(entityAddress + offsets::PositionY);
			float posZ = pm.ReadMemory<float>(entityAddress + offsets::PositionZ);
			// Выводим координаты игрока
			std::cout << "Игрок " << i + 1 << ": "
				<< "X: " << posX << ", "
				<< "Y: " << posY << ", "
				<< "Z: " << posZ << std::endl;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void updateHealth(ProcessMemory& pm, DWORD lp) {
	while (true) {
		pm.WriteMemory<DWORD>(offsets::HealthValue + lp, 1000);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

int main()
{
	setlocale(LC_ALL, "");
	// получение дескрипторов
	HWND hwnd = FindWindow(nullptr, TEXT("AssaultCube"));
	if (hwnd == nullptr) {
		std::cerr << "Окно не найдено!" << std::endl;
		return 1;
	}
	DWORD processId = 0;
	GetWindowThreadProcessId(hwnd, &processId);
	if (processId != 0) {
		std::cout << "Process ID (PID): " << processId << std::endl;
	}
	else {
		std::cerr << "Не удалось получить PID!" << std::endl;
	}
	
	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	if (processHandle == nullptr || processHandle == INVALID_HANDLE_VALUE) {
		std::cerr << "Не удалось открыть процесс. Код ошибки: " << GetLastError() << std::endl;
		return 1;
	}


	ProcessMemory pm(processHandle);
	DWORD lp = pm.ReadMemory<DWORD>(offsets::LocalPlayer);
	DWORD cp = pm.ReadMemory<DWORD>(offsets::PlayerCount);
	DWORD entityList = pm.ReadMemory<DWORD>(offsets::EntityList);

	std::thread playerThread(monitorPlayers, std::ref(pm), cp, entityList);
	std::thread healthThread(updateHealth, std::ref(pm), lp);
	playerThread.join();
	healthThread.join();

}