#pragma once
#include <windows.h>
#include <psapi.h>
#include <string>
#include <tlhelp32.h>
#include <iostream>

class memory {
public:
	auto attach(std::string process) -> bool {
		DWORD process_id = 0;
		auto info = PROCESSENTRY32{};
		info.dwSize = sizeof(info);

		auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		if (snapshot == INVALID_HANDLE_VALUE)
			return false;

		Process32First(snapshot, &info);
		if (!process.compare(info.szExeFile)) {
			process_id = info.th32ProcessID;
		}

		while (Process32Next(snapshot, &info)) {
			if (!process.compare(info.szExeFile)) {
				process_id = info.th32ProcessID;
			}
		}

		CloseHandle(snapshot);
		_pid = process_id;
		return _handle = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, false, process_id);
	}

	template <class T>
	auto read(DWORD address) -> T {
		T buf;
		ReadProcessMemory(_handle, reinterpret_cast<LPVOID>(address), &buf, sizeof(T), nullptr);

		return buf;
	}

	template <class T>
	auto write(T value, DWORD address) -> void {
		WriteProcessMemory(_handle, reinterpret_cast<LPVOID>(address), &value, sizeof(T), nullptr);
	}

	auto get_module(std::string module, DWORD &size) -> DWORD {
		auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, _pid);
		auto entry = MODULEENTRY32{};
		entry.dwSize = sizeof(entry);

		while (Module32Next(snapshot, &entry)) {
			if (!strcmp(entry.szModule, module.c_str())) {
				std::cout << "Found " + module + " at: 0x" << std::hex << reinterpret_cast<DWORD>(entry.modBaseAddr) << std::dec << std::endl;

				CloseHandle(snapshot);
				size = entry.modBaseSize;
				return reinterpret_cast<DWORD>(entry.modBaseAddr);
			}
		}

		return 0;
	}

	auto find_pattern(DWORD base, DWORD length, const char *pattern, const char *mask) -> DWORD {
		unsigned char *buf = reinterpret_cast<unsigned char*>(VirtualAlloc(0, length, MEM_COMMIT, PAGE_EXECUTE_READWRITE));
		const unsigned char *sig = reinterpret_cast<const unsigned char*>(pattern);

		if (!ReadProcessMemory(_handle, reinterpret_cast<LPVOID>(base), buf, length, NULL)) {
			return 0;
		}

		for (auto i = 0; i < length; i++) {
			if ((buf[i] == sig[0] && mask[0] == 'x') || (mask[0] == '?')) {
				for (auto x = 0;; x++) {
					if (mask[x] == 'x') {
						if (buf[i + x] == sig[x]) {
							continue;
						} else {
							break;
						}
					} else if (mask[x] == 0x00) {
						return (DWORD)((base + i));
					}
				}
			}
		}

		return 0;
	}

	auto close() -> void {
		CloseHandle(_handle);
	}

private:
	std::uintptr_t _pid;
	HANDLE _handle;
};

extern memory mem;