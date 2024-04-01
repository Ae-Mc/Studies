#include <bitset>
#include <errhandlingapi.h>
#include <iostream>
#include <map>
#include <memoryapi.h>
#include <minwindef.h>
#include <ostream>
#include <string>
#include <sysinfoapi.h>
#include <windows.h>
#include <winnt.h>

using namespace std;

enum MenuEntry {
  MENU_SYSTEM_INFO,
  MENU_MEMORY_STATUS,
  MENU_MEMORY_REGION_STATUS,
  MENU_REGION_RESERVE,
  MENU_MEMORY_WRITE,
  MENU_REGION_SECURE,
  MENU_MEMORY_FREE,
  MENU_EXIT,
};

constexpr const char *menu_entries[] = {
    "1. Получение информации о вычислительной системе",
    "2. Определение статуса виртуальной памяти",
    "3. Определение состояния участка памяти",
    "4. Резервирование региона",
    "5. Запись данных в ячейки памяти",
    "6. Установка защиты доступа региона памяти",
    "7. Возврат физической памяти",
    "e. Выход"};

void print_menu();
MenuEntry select_menu();
void system_info();
void memory_status();
void memory_region_status();
void region_reserve();
void memory_write();
void region_secure();
void memory_free();

constexpr void (*const menu_functions[])() = {
    system_info,  memory_status, memory_region_status, region_reserve,
    memory_write, region_secure, memory_free,
};

int main() {
  MenuEntry selection;
  do {
    print_menu();
    selection = select_menu();
    if (selection != MENU_EXIT) {
      menu_functions[selection]();
    }
  } while (selection != MENU_EXIT);
}

void print_menu() {
  for (auto entry : menu_entries) {
    cout << entry << endl;
  }
}

MenuEntry select_menu() {
  char selection;
  do {
    cin >> selection;
  } while (!(selection >= MENU_SYSTEM_INFO && selection <= MENU_EXIT &&
             selection != 'e'));
  if (selection == 'e') {
    return MenuEntry::MENU_EXIT;
  }
  return MenuEntry(selection);
}

void system_info() {
  static map<int, string> architecture_mapping = {
      {0, "x86"},   {5, "ARM"},    {6, "IA64"},
      {9, "AMD64"}, {12, "ARM64"}, {0xffff, "Unknown"},
  };
  SYSTEM_INFO info;
  GetSystemInfo(&info);
  cout << "Page size: " << info.dwPageSize << endl
       << hex
       << "Minimum allocation address: " << info.lpMinimumApplicationAddress
       << dec << endl
       << "Maximum allocation address: " << info.lpMaximumApplicationAddress
       << endl
       << "Active processor mask: " << bitset<64>(info.dwActiveProcessorMask)
       << endl
       << "Number of logical processors: " << info.dwNumberOfProcessors << endl
       << "Allocation granularity: " << info.dwAllocationGranularity << endl
       << "Processor level: " << info.wProcessorLevel << endl
       << "Processor revision: " << hex << info.wProcessorRevision << dec
       << endl
       << "Processor architecture: "
       << architecture_mapping[info.wProcessorArchitecture] << endl;
  cin.get();
}

void memory_status() {
  MEMORYSTATUSEX status;

  status.dwLength = sizeof(status);

  if (GlobalMemoryStatusEx(&status)) {
    cout << "Memory used (%): " << status.dwMemoryLoad << endl
         << "Total physical memory (bytes): " << status.ullTotalPhys << endl
         << "Available physics memory (bytes): " << status.ullAvailPhys << endl
         << "Total page file (bytes): " << status.ullTotalPageFile << endl
         << "Available page file (bytes): " << status.ullAvailPageFile << endl
         << "Total virtual (bytes): " << status.ullTotalVirtual << endl
         << "Available virtual (bytes): " << status.ullAvailVirtual << endl;
  } else {
    cout << "Error getting global memory status: " << hex << GetLastError()
         << dec << GetLastError() << endl;
  }
  cin.get();
}

void memory_region_status() {
  LPVOID region_address;
  MEMORY_BASIC_INFORMATION info;
  cout << "Enter region address: ";
  cin >> region_address;

  if (VirtualQuery(region_address, &info, sizeof(info))) {
    cout << "Region base address: " << info.BaseAddress << endl << ""
  }
}