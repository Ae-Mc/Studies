#include "consts.hpp"
#include "general.hpp"
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <stdexcept>
#include <string>

#include <sys/fcntl.h>
#include <sys/mman.h>
#include <vector>

using namespace std;

const vector<const char *> menu_entries = {
    "1. Выполнить проецирование",
    "2. Записать данные",
    "3. Завершить работу",
};

struct Projection {
  int fd; // File descriptor
  uintmax_t file_size;
  char *start_ptr, *current_ptr;
};

Projection perform_projection();
void write_data(Projection &projection);
void shutdown(Projection &projection);

int main() {
  int selection;
  Projection projection{};
  do {
    menu_general(menu_entries);
    selection = menu_select(1, 3);
    switch (selection) {
    case 1:
      projection = perform_projection();
      break;
    case 2:
      write_data(projection);
      break;
    }
  } while (selection != 3);

  shutdown(projection);
  return 0;
}

Projection perform_projection() {
  Projection projection;
  projection.fd = open(FILENAME, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
  if (projection.fd == 0) {
    throw runtime_error("Error opening file");
  }
  projection.file_size = filesystem::file_size(FILENAME);
  projection.start_ptr = projection.current_ptr =
      (char *)(mmap(NULL, projection.file_size, PROT_READ | PROT_WRITE,
                    MAP_SHARED, projection.fd, 0));
  if (projection.start_ptr == NULL) {
    throw runtime_error("Error projecting file");
  }

  return projection;
}

void write_data(Projection &projection) {
  string buffer;
  cout << "Введите данные для файла. Для окончания ввода введите пустую строку:"
       << endl;
  while (getline(cin, buffer).good()) {
    if (buffer.empty()) {
      break;
    }
    buffer += '\n';
    memcpy(projection.current_ptr, buffer.c_str(), buffer.size());
    projection.current_ptr += buffer.size();
  }
}

void shutdown(Projection &projection) {
  munmap(projection.start_ptr, projection.file_size);
  filesystem::remove(FILENAME);
}