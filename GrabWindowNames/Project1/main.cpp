
#include <iostream>
#include <string>
#include <vector>

#include <windows.h>
#include <tlhelp32.h>

bool isNotepad(const PROCESSENTRY32W& entry) {
    return std::wstring(entry.szExeFile) == L"notepad.exe";
}

BOOL CALLBACK enumWindowsProc(HWND hwnd, LPARAM lParam) {
    const auto& pids = *reinterpret_cast<std::vector<DWORD>*>(lParam);

    DWORD winId;
    GetWindowThreadProcessId(hwnd, &winId);

    for (DWORD pid : pids) {
        if (winId == pid) {
            std::wstring title(GetWindowTextLength(hwnd) + 1, L'\0');
            GetWindowTextW(hwnd, &title[0], title.size()); //note: C++11 only
            if (pid == 8640) {
                std::cout << "aaa:\n";
            }
            std::cout << "Found window:\n";
            std::cout << "Process ID: " << pid << '\n';
            std::wcout << "Title: " << title << "\n\n";
        }
    }

    return TRUE;
}

int main() {
    std::vector<DWORD> pids;

    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    PROCESSENTRY32W entry;
    entry.dwSize = sizeof entry;

    if (!Process32FirstW(snap, &entry)) {
        return 0;
    }

    do {
      //  if (isNotepad(entry)) {
            pids.emplace_back(entry.th32ProcessID);
       // }
    } while (Process32NextW(snap, &entry));

    EnumWindows(enumWindowsProc, reinterpret_cast<LPARAM>(&pids));
}