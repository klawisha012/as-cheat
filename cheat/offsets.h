#pragma once
#include <Windows.h>

namespace offsets
{
	DWORD viewMatrixAddress = 0x57DFDC;
	DWORD LocalPlayer = 0x00400000 + 0x0017E0A8;
	DWORD EntityList = 0x00400000 + 0x18AC04;
	DWORD FOV = 0x00400000 + 0x18A7CC;
	DWORD PlayerCount = 0x00400000 + 0x18AC0C;
	DWORD PositionX = 0x2C;
	DWORD PositionY = 0x30;
	DWORD PositionZ = 0x28;
	DWORD HeadPositionX = 0x4;
	DWORD HeadPositionY = 0xC;
	DWORD HeadPositionZ = 0x8;
	DWORD PlayerCameraX = 0x34;
	DWORD PlayerCameraY = 0x38;
	DWORD AssaultRifleAmmo = 0x140;
	DWORD SubmachineGunAmmo = 0x138;
	DWORD SniperAmmo = 0x13C;
	DWORD Shotgun = 0x134;
	DWORD PistolAmmo = 0x12C;
	DWORD GrenadeAmmo = 0x144;
	DWORD FastfireAssaultRifle = 0x164;
	DWORD FastfireSniper = 0x160;
	DWORD FastfireShotgun = 0x158;
	DWORD Autoshoot = 0x204;
	DWORD HealthValue = 0xEC;
	DWORD ArmorValue = 0xF0;
	DWORD PlayerName = 0x205;
}