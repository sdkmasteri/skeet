#pragma once
#include <Windows.h>

typedef struct _PEB_LDR_DATA {
    ULONG Length;                   // +0x000 - ����� ���������
    BOOLEAN Initialized;            // +0x004 - ���� �������������
    PVOID SsHandle;                 // +0x008 - ���������������
    PLIST_ENTRY InLoadOrderModuleList; // +0x00C - ���������� ������ ������� � ������� ��������
    PLIST_ENTRY InMemoryOrderModuleList; // +0x014 - ���������� ������ ������� � ������� ������������ � ������
    PLIST_ENTRY InInitializationOrderModuleList; // +0x01C - ���������� ������ ������� � ������� �������������
    PVOID EntryInProgress;          // +0x024 - ������, ������� ��������� � �������� ��������
    BOOLEAN ShutdownInProgress;     // +0x028 - ���� ���������� ������
    PVOID ShutdownThreadId;         // +0x02C - ������������� ������, ����������� �� ���������� ������
} PEB_LDR_DATA, * PPEB_LDR_DATA;

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR Buffer;
} UNICODE_STRING;


typedef struct _LDR_DATA_TABLE_ENTRY {
    PLIST_ENTRY InLoadOrderLinks;         // +0x000: ��������� ��� ����������� ������ ������� � ������� ��������
    PLIST_ENTRY InMemoryOrderLinks;       // +0x008: ��������� ��� ����������� ������ ������� � ������� �� ������������ � ������
    PLIST_ENTRY InInitializationOrderLinks; // +0x010: ��������� ��� ����������� ������ ������� � ������� �������������
    PVOID DllBase;                       // +0x018: ������� ����� ������������ ������ (DLL ��� .exe)
    PVOID EntryPoint;                    // +0x01C: ����� ����� � ������ (���� ���������)
    ULONG SizeOfImage;                   // +0x020: ������ ������������ ������ � ������
    UNICODE_STRING FullDllName;          // +0x024: ������ ��� ������ (� ����)
    UNICODE_STRING BaseDllName;          // +0x02C: �������� ��� ������ (��� ����)
    ULONG Flags;                         // +0x034: �����, ����������� ��������� ������
    SHORT LoadCount;                     // +0x038: ������� �������� ������ (���������� ������ �� ������)
    SHORT TlsIndex;                      // +0x03A: ������ � ������� ��������� ���������� ������ (TLS)
    PLIST_ENTRY HashLinks;                // +0x03C: ������ ��� ���-������ ����������
    PVOID SectionPointer;                // +0x044: ��������� �� ������ ������ (���� ���������)
    ULONG CheckSum;                      // +0x048: ����������� ����� ������
    ULONG TimeDateStamp;                 // +0x04C: ��������� ����� ���������� (���� �������� ������)
} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

typedef struct _RTL_USER_PROCESS_PARAMETERS {
    ULONG MaximumLength;               // +0x000: ������������ ����� ������
    ULONG Length;                      // +0x004: ������� ����� ���������
    ULONG Flags;                       // +0x008: �����, ������������ ��������� �������� (��������, ������������)
    ULONG DebugFlags;                  // +0x00C: ����� ������� (������ 0)
    HANDLE ConsoleHandle;              // +0x010: ���������� �������, ������������ ���������
    ULONG ConsoleFlags;                // +0x014: ����� ������� (��������, �������� �� �������)
    HANDLE StandardInput;              // +0x018: ���������� ������������ �����
    HANDLE StandardOutput;             // +0x01C: ���������� ������������ ������
    HANDLE StandardError;              // +0x020: ���������� ������������ ������ ������
    UNICODE_STRING CurrentDirectoryPath; // +0x024: ������� ������� �������
    HANDLE CurrentDirectoryHandle;     // +0x02C: ���������� �������� ��������
    UNICODE_STRING DllPath;            // +0x030: ���� � ����������� ����������� (DLL)
    UNICODE_STRING ImagePathName;      // +0x038: ���� � ������������ ����� (EXE)
    UNICODE_STRING CommandLine;        // +0x040: ��������� ������, ���������� ��������
    PVOID Environment;                 // +0x048: ��������� �� ���������� ���������
    ULONG StartingX;                   // +0x04C: ��������� ���������� X ���� ������� (���� ���������)
    ULONG StartingY;                   // +0x050: ��������� ���������� Y ���� ������� (���� ���������)
    ULONG CountX;                      // +0x054: ������ ���� ������� � ��������
    ULONG CountY;                      // +0x058: ������ ���� ������� � ��������
    ULONG CountCharsX;                 // +0x05C: ������ ������ ������� � ��������
    ULONG CountCharsY;                 // +0x060: ������ ������ ������� � ��������
    ULONG FillAttribute;               // +0x064: �������� ������� (���� ���� � ������)
    ULONG WindowFlags;                 // +0x068: �����, ������������ ��������� ���� �������
    ULONG ShowWindowFlags;             // +0x06C: ����� ��� ������ ����
    UNICODE_STRING WindowTitle;        // +0x070: ��������� ����
    UNICODE_STRING DesktopInfo;        // +0x078: ���������� � ������� ����� (��� ��������)
    UNICODE_STRING ShellInfo;          // +0x080: ���������� � �������� (������ `cmd.exe`)
    UNICODE_STRING RuntimeData;        // +0x088: ������, ������������ �� ����� ����������
    PVOID CurrentDirectories[32];      // +0x090: ������ ������� ��������� ��� ������� �����
    ULONG EnvironmentSize;             // +0x0B0: ������ ���������� ���������
    ULONG EnvironmentVersion;          // +0x0B4: ������ �����
    PVOID PackageDependencyData;       // +0x0B8: ������ ����������� ������
    ULONG ProcessGroupId;              // +0x0BC: ������������� ������ ���������
    ULONG LoaderThreads;               // +0x0C0: ���������� ������� ����������
    UNICODE_STRING RedirectionDllName; // +0x0C8: ��� DLL ��� ��������� (���� ������������)
    UNICODE_STRING HeapPartitionName;  // +0x0D0: ��� ������� ����
    ULONG64 DefaultThreadpoolCpuSetMasks; // +0x0D8: ����� CPU ��� �������
    ULONG DefaultThreadpoolCpuSetMaskCount; // +0x0E0: ���������� ����� CPU ��� �������
} RTL_USER_PROCESS_PARAMETERS, * PRTL_USER_PROCESS_PARAMETERS;


typedef struct _PEB {
    BOOLEAN InheritedAddressSpace;   // +0x000: ���������, ��� �� �������� ����� ����������� �� ������������� ��������
    BOOLEAN ReadImageFileExecOptions; // +0x001: ���������, ���� �� ��������� ��������� ���������� ������
    BOOLEAN BeingDebugged;           // +0x002: ���������, ����������� �� ������� ��� ����������
    BOOLEAN SpareBool;               // +0x003: ����������������� ����
    HANDLE Mutant;                   // +0x004: ��������� �� ������-������� �������������
    PVOID ImageBaseAddress;          // +0x008: ������� ����� ������ ��������
    PPEB_LDR_DATA Ldr;               // +0x00C: ��������� �� ��������� ���������� ������� (PEB_LDR_DATA)
    PRTL_USER_PROCESS_PARAMETERS ProcessParameters; // +0x010: ��������� �� ��������� �������� (��������, ���������� �����, ��������� ������)
    PVOID SubSystemData;             // +0x014: ��������� �� ������ ����������
    PVOID ProcessHeap;               // +0x018: ��������� �� �������� ������� ������� ��������
    PVOID FastPebLock;               // +0x01C: ��������� �� ������ ������������� PEB
    PVOID FastPebLockRoutine;        // +0x020: ��������� �� ������� ���������� PEB
    PVOID FastPebUnlockRoutine;      // +0x024: ��������� �� ������� ������������� PEB
    ULONG EnvironmentUpdateCount;    // +0x028: ������� ���������� ���������� �����
    PVOID KernelCallbackTable;       // +0x02C: ��������� �� ������� �������� ������� ����
    ULONG SystemReserved[1];         // +0x030: ����������������� ������
    ULONG AtlThunkSListPtr32;        // +0x034: ���������������
    PVOID FreeList;                  // +0x038: ���������������
    ULONG TlsExpansionCounter;       // +0x03C: ������� ���������� TLS
    PVOID TlsBitmap;                 // +0x040: ��������� �� ����� ����� TLS
    ULONG TlsBitmapBits[2];          // +0x044: ���� ����� TLS
    PVOID ReadOnlySharedMemoryBase;  // +0x04C: ��������� �� ���� ������, ��������� ������ ��� ������
    PVOID ReadOnlySharedMemoryHeap;  // +0x050: ��������� �� ���� ��� ������ ������ ��� ������
    PVOID ReadOnlyStaticServerData;  // +0x054: ��������� �� ������ �������
    PVOID AnsiCodePageData;          // +0x058: ��������� �� ������ ������� �������� ANSI
    PVOID OemCodePageData;           // +0x05C: ��������� �� ������ OEM ������� ��������
    PVOID UnicodeCaseTableData;      // +0x060: ��������� �� ������� �������������� �������� Unicode
    ULONG NumberOfProcessors;        // +0x064: ���������� �����������
    ULONG NtGlobalFlag;              // +0x068: ���������� ����� NT, ������������� ��� ��������
    LARGE_INTEGER CriticalSectionTimeout; // +0x070: ����� �������� ��� ����������� ������
    ULONG HeapSegmentReserve;        // +0x078: ����������������� ������ �������� ����
    ULONG HeapSegmentCommit;         // +0x07C: ��������������� ������ �������� ����
    ULONG HeapDeCommitTotalFreeThreshold; // +0x080: ����� ����������� ��������� ������ � ����
    ULONG HeapDeCommitFreeBlockThreshold; // +0x084: ����� ����������� ��������� ������ � ����
    ULONG NumberOfHeaps;             // +0x088: ���������� ��� � ��������
    ULONG MaximumNumberOfHeaps;      // +0x08C: ������������ ���������� ��� � ��������
    PVOID* ProcessHeaps;             // +0x090: ��������� �� ������ ���������� �� ���� ��������
    PVOID GdiSharedHandleTable;      // +0x094: ��������� �� ������� GDI
    PVOID ProcessStarterHelper;      // +0x098: ��������� �� ��������� ������� ��������
    ULONG GdiDCAttributeList;        // +0x09C: �������� GDI
    PVOID LoaderLock;                // +0x0A0: ���������� ���������� �������
    ULONG OSMajorVersion;            // +0x0A4: �������� ������ ������������ �������
    ULONG OSMinorVersion;            // +0x0A8: �������� ������ ������������ �������
    ULONG OSBuildNumber;             // +0x0AC: ����� ������ ������������ �������
    ULONG OSPlatformId;              // +0x0B0: ������������� ���������
    ULONG ImageSubsystem;            // +0x0B4: ����������, � ������� �������� �����
    ULONG ImageSubsystemMajorVersion; // +0x0B8: �������� ������ ����������
    ULONG ImageSubsystemMinorVersion; // +0x0BC: �������� ������ ����������
    ULONG ActiveProcessAffinityMask; // +0x0C0: ����� �������� ��������� ��������
    ULONG GdiHandleBuffer[34];       // +0x0C4: ����� ��� ������� GDI
    PVOID PostProcessInitRoutine;    // +0x14C: ��������� �� ������� ������������� ����� ������� ��������
    PVOID TlsExpansionBitmap;        // +0x150: ��������� �� ���������� ������� ����� TLS
    ULONG TlsExpansionBitmapBits[32]; // +0x154: ���� ����������� ����� TLS
    ULONG SessionId;                 // +0x1D4: ������������� ������
} PEB, * PPEB;