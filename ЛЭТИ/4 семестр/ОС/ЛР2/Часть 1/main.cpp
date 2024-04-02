#include <bitset>
#include <cstddef>
#include <errhandlingapi.h>
#include <functional>
#include <iostream>
#include <map>
#include <memoryapi.h>
#include <minwindef.h>
#include <numeric>
#include <ostream>
#include <ranges>
#include <set>
#include <sstream>
#include <string>
#include <sysinfoapi.h>
#include <windows.h>
#include <winnt.h>

using namespace std;

enum MenuEntry {
  MENU_SYSTEM_INFO = '1',
  MENU_MEMORY_STATUS = '2',
  MENU_MEMORY_REGION_STATUS = '3',
  MENU_REGION_RESERVE = '4',
  MENU_MEMORY_WRITE = '5',
  MENU_REGION_PROTECT = '6',
  MENU_MEMORY_FREE = '7',
  MENU_EXIT = 'e',
};

const vector<tuple<const char *, DWORD>> MEMORY_PROTECTION_CONSTANTS{
    {"PAGE_EXECUTE", 0x10},
    {"PAGE_EXECUTE_READ", 0x20},
    {"PAGE_EXECUTE_READWRITE", 0x40},
    {"PAGE_EXECUTE_WRITECOPY", 0x80},
    {"PAGE_NOACCESS", 0x01},
    {"PAGE_READONLY", 0x02},
    {"PAGE_READWRITE", 0x04},
    {"PAGE_WRITECOPY", 0x08},
    {"PAGE_TARGETS_NO_UPDATE", 0x40000000},
    {"PAGE_GUARD", 0x100},
    {"PAGE_NOCACHE", 0x200},
    {"PAGE_WRITECOMBINE", 0x400},
};

const DWORD MEMORY_PROTECTION_BITS =
    reduce((MEMORY_PROTECTION_CONSTANTS | ranges::views::values).begin(),
           (MEMORY_PROTECTION_CONSTANTS | ranges::views::values).end(), 0,
           [](DWORD a, DWORD b) { return a |= b; });

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
void region_protect();
void memory_free();

constexpr void (*const menu_functions[])() = {
    system_info,  memory_status,  memory_region_status, region_reserve,
    memory_write, region_protect, memory_free,
};

int main() {
  MenuEntry selection;
  cout << hex << MEMORY_PROTECTION_BITS << endl << dec;
  do {
    print_menu();
    selection = select_menu();
    if (selection != MENU_EXIT) {
      menu_functions[selection - '1']();
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
  } while (!((selection >= MENU_SYSTEM_INFO && selection <= MENU_MEMORY_FREE) ||
             selection == MENU_EXIT));
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
  static const map<int, const char *> MEMORY_STATES = {
      {0x1000, "MEM_COMMIT"},
      {0x2000, "MEM_RESERVE"},
      {0x10000, "MEM_FREE"},
  };

  static const map<int, const char *> MEMORY_TYPES = {
      {0x20000, "MEM_PRIVATE"},
      {0x40000, "MEM_MAPPED"},
      {0x1000000, "MEM_IMAGE"},
  };

  static const auto print_memory_protection = [](int protection_value) {
    for (const auto &[name, value] : MEMORY_PROTECTION_CONSTANTS) {
      if ((protection_value & value) != 0) {
        cout << '\t' << name << endl;
      }
    }
  };

  LPVOID region_address;
  MEMORY_BASIC_INFORMATION info;
  cout << "Enter region address: ";
  cin >> region_address;

  if (VirtualQuery(region_address, &info, sizeof(info))) {
    cout << "Region base address: " << info.BaseAddress << endl
         << "Allocation base: " << info.AllocationBase << endl
         << "Region allocation protection:" << endl;
    print_memory_protection(info.AllocationProtect);
    cout << "Partition ID: " << info.PartitionId << endl
         << "Region size (bytes): " << info.RegionSize << endl
         << "Region pages state: " << MEMORY_STATES.at(info.State) << endl
         << "Region current protection:" << endl;
    print_memory_protection(info.Protect);
    cout << "Region type: " << MEMORY_TYPES.at(info.Type) << endl;
  }
  cin.get();
}

void region_reserve() {
  cout << "Enter region address (leave empty to decide automatically): ";
  string address_str;
  cin >> address_str;
  LPVOID address = NULL;
  if (!address_str.empty()) {
    if (!(istringstream(address_str) >> address).good()) {
      cout << "Error reading address from input. Deducing automatically..."
           << endl;
    }
  }
  cout << "Enter region size: ";
  SIZE_T size;
  cin >> size;

  DWORD allocation_type, filtered;
  static const vector<DWORD> ALLOCATION_TYPES{
      0x1000,     0x2000,   0x80000,  1000000,
      0x20000000, 0x400000, 0x100000, 0x200000,
  };
  do {
    cout << "Enter allocation type: ";
    cin >> allocation_type;
    filtered =
        reduce(ALLOCATION_TYPES.begin(), ALLOCATION_TYPES.end(),
               allocation_type, [](DWORD a, DWORD b) { return b &= ~a; });
    if (filtered != 0) {
      cout << "Error! Wrong allocation type" << endl;
    }
  } while (filtered != 0);

  DWORD protect;
  do {
    cout << "Enter protection constant: ";
    cin >> protect;
    if ((protect & MEMORY_PROTECTION_BITS) != protect) {
      cout << "Error! Wrong constant" << endl;
    }
  } while ((protect & MEMORY_PROTECTION_BITS) != protect);

  auto ptr = VirtualAlloc(address, size, allocation_type, protect);
  if (ptr == NULL) {
    cout << "Error allocating memory. Error code: " << GetLastError();
  } else {
    cout << "Allocated region base address: " << ptr << endl;
  }
}

void memory_write() {
  cout << "Enter pairs of addresses and values (DWORD). Leave input empty to "
          "exit"
       << endl;
  string line;
  do {
    LPVOID address;
    DWORD value;
    if (getline(cin, line)) {
      if ((istringstream(line) >> address >> value).good()) {
        *(DWORD *)address = value;
      } else {
        break;
      }
    }
  } while (true);
  cin.get();
}

void region_protect() {
  cout << "Enter region address (leave empty to decide automatically): ";
  LPVOID address;
  cin >> address;

  cout << "Enter region size: ";
  SIZE_T size;
  cin >> size;

  DWORD protect;
  do {
    cout << "Enter protection constant: ";
    cin >> protect;
    if ((protect & MEMORY_PROTECTION_BITS) != protect) {
      cout << "Error! Wrong constant" << endl;
    }
  } while ((protect & MEMORY_PROTECTION_BITS) != protect);

  PDWORD old_protect = nullptr;

  if (!VirtualProtect(address, size, protect, old_protect)) {
    cout << "Error protecting region. Error code: " << GetLastError() << endl;
  }
  cin.get();
}

void memory_free() {
  cout << "Enter region address (leave empty to decide automatically): ";
  LPVOID address;
  cin >> address;

  cout << "Enter region size: ";
  SIZE_T size;
  cin >> size;

  static const set<DWORD> FREE_TYPES = {0x4000, 0x8000, 1, 2};

  DWORD free_type, filtered;
  do {
    cin >> free_type;
    filtered = reduce(begin(FREE_TYPES), end(FREE_TYPES), free_type,
                      [](DWORD a, DWORD b) { return a & ~b; });
  } while (filtered != 0);

  if (!VirtualFree(address, size, free_type)) {
    cout << "Error freeing memory. Error code: " << GetLastError() << endl;
  }
}

template <typename T, typename M1, typename M2, typename... Args>
function<T(Args...)> operator|(function<M1(Args...)> inner,
                               function<T(M2)> outer) {
  return [&](Args... args) { return outer(inner(args...)); };
}
