#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <filesystem>
#include "skCrypter.h"

static HANDLE GetProcessByName(const std::wstring& name)
{
  DWORD pid = 0;

  // Create toolhelp snapshot.
  HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  PROCESSENTRY32 process;
  ZeroMemory(&process, sizeof(process));
  process.dwSize = sizeof(process);

  // Walkthrough all processes.
  if (Process32First(snapshot, &process))
  {
    do
    {
      // Compare process.szExeFile based on format of name, i.e., trim file path
      // trim .exe if necessary, etc.
      if (std::wstring(process.szExeFile) == std::wstring(name))
      {
        pid = process.th32ProcessID;
        break;
      }
    } while (Process32Next(snapshot, &process));
  }

  CloseHandle(snapshot);

  if (pid != 0)
  {
    return OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
  }
  return INVALID_HANDLE_VALUE;
}

int main()
{
  SetConsoleTitleA(skCrypt("DONT RENAME skeet.dll!"));
  std::cout << skCrypt("DONT RENAME skeet.dll!\nEnter an -insecure in csgo params\nwaiting for csgo...\n");
  HANDLE hProc = INVALID_HANDLE_VALUE;
  auto path = std::filesystem::current_path();
  auto pathStr = path.wstring() + L"\\skeet.dll";
  while (hProc == INVALID_HANDLE_VALUE) {
    hProc = GetProcessByName(L"csgo.exe");
  }
  auto cheat = VirtualAllocEx(hProc, (void*)0x43310000, 0x2fc000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

  if (!cheat)
    std::cout << skCrypt("failed to allocate cheat!\n");

  auto arg = VirtualAllocEx(hProc, 0, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
  WriteProcessMemory(hProc, arg, pathStr.c_str(), pathStr.length() * 2, nullptr);
  auto hThread = CreateRemoteThread(hProc, 0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(GetProcAddress(GetModuleHandleA(skCrypt("kernel32.dll")), skCrypt("LoadLibraryW"))), arg, 0, 0);
  std::cout << skCrypt("csgo.exe found!\n");
  WaitForSingleObject(hThread, INFINITE);
}