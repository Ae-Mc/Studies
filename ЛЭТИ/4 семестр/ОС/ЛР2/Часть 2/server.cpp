#include "consts.hpp"
#include <errhandlingapi.h>
#include <fileapi.h>
#include <handleapi.h>
#include <iostream>
#include <string>
#include <windows.h>
#include <winnt.h>

using namespace std;

int main() {
  HANDLE hFile = CreateFileA(FILENAME, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                             FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile == NULL) {
    cerr << "Error opening file. Error code: " << GetLastError();
    return GetLastError();
  }
  cout << "Enter text lines (empty line to end): " << endl;

  string buf;
  DWORD bytesWritten;
  while (getline(cin, buf).good()) {
    if (buf.empty()) {
      break;
    }
    cout << buf;
    if (!WriteFile(hFile, buf.c_str(), buf.size(), &bytesWritten, NULL)) {
      cerr << "Error writing to file. Error code: " << GetLastError();
      CloseHandle(hFile);
      return GetLastError();
    }
  }

  CloseHandle(hFile);
  return 0;
}