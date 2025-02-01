#pragma once
#include <Windows.h>

typedef struct _PEB_LDR_DATA {
    ULONG Length;                   // +0x000 - Длина структуры
    BOOLEAN Initialized;            // +0x004 - Флаг инициализации
    PVOID SsHandle;                 // +0x008 - Зарезервировано
    PLIST_ENTRY InLoadOrderModuleList; // +0x00C - Двусвязный список модулей в порядке загрузки
    PLIST_ENTRY InMemoryOrderModuleList; // +0x014 - Двусвязный список модулей в порядке расположения в памяти
    PLIST_ENTRY InInitializationOrderModuleList; // +0x01C - Двусвязный список модулей в порядке инициализации
    PVOID EntryInProgress;          // +0x024 - Модуль, который находится в процессе загрузки
    BOOLEAN ShutdownInProgress;     // +0x028 - Флаг завершения работы
    PVOID ShutdownThreadId;         // +0x02C - Идентификатор потока, отвечающего за завершение работы
} PEB_LDR_DATA, * PPEB_LDR_DATA;

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR Buffer;
} UNICODE_STRING;


typedef struct _LDR_DATA_TABLE_ENTRY {
    PLIST_ENTRY InLoadOrderLinks;         // +0x000: Указатели для двусвязного списка модулей в порядке загрузки
    PLIST_ENTRY InMemoryOrderLinks;       // +0x008: Указатели для двусвязного списка модулей в порядке их расположения в памяти
    PLIST_ENTRY InInitializationOrderLinks; // +0x010: Указатели для двусвязного списка модулей в порядке инициализации
    PVOID DllBase;                       // +0x018: Базовый адрес загруженного модуля (DLL или .exe)
    PVOID EntryPoint;                    // +0x01C: Точка входа в модуль (если применимо)
    ULONG SizeOfImage;                   // +0x020: Размер загруженного образа в памяти
    UNICODE_STRING FullDllName;          // +0x024: Полное имя модуля (с путём)
    UNICODE_STRING BaseDllName;          // +0x02C: Короткое имя модуля (без пути)
    ULONG Flags;                         // +0x034: Флаги, описывающие состояние модуля
    SHORT LoadCount;                     // +0x038: Счётчик загрузок модуля (количество ссылок на модуль)
    SHORT TlsIndex;                      // +0x03A: Индекс в таблице локальных переменных потока (TLS)
    PLIST_ENTRY HashLinks;                // +0x03C: Ссылки для хеш-таблиц загрузчика
    PVOID SectionPointer;                // +0x044: Указатель на объект секции (если применимо)
    ULONG CheckSum;                      // +0x048: Контрольная сумма модуля
    ULONG TimeDateStamp;                 // +0x04C: Временная метка компиляции (дата создания модуля)
} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

typedef struct _RTL_USER_PROCESS_PARAMETERS {
    ULONG MaximumLength;               // +0x000: Максимальная длина буфера
    ULONG Length;                      // +0x004: Текущая длина структуры
    ULONG Flags;                       // +0x008: Флаги, определяющие состояние процесса (например, наследование)
    ULONG DebugFlags;                  // +0x00C: Флаги отладки (обычно 0)
    HANDLE ConsoleHandle;              // +0x010: Дескриптор консоли, используемой процессом
    ULONG ConsoleFlags;                // +0x014: Флаги консоли (например, активная ли консоль)
    HANDLE StandardInput;              // +0x018: Дескриптор стандартного ввода
    HANDLE StandardOutput;             // +0x01C: Дескриптор стандартного вывода
    HANDLE StandardError;              // +0x020: Дескриптор стандартного вывода ошибок
    UNICODE_STRING CurrentDirectoryPath; // +0x024: Текущий рабочий каталог
    HANDLE CurrentDirectoryHandle;     // +0x02C: Дескриптор текущего каталога
    UNICODE_STRING DllPath;            // +0x030: Путь к загружаемым библиотекам (DLL)
    UNICODE_STRING ImagePathName;      // +0x038: Путь к исполняемому файлу (EXE)
    UNICODE_STRING CommandLine;        // +0x040: Командная строка, переданная процессу
    PVOID Environment;                 // +0x048: Указатель на переменные окружения
    ULONG StartingX;                   // +0x04C: Начальная координата X окна консоли (если применимо)
    ULONG StartingY;                   // +0x050: Начальная координата Y окна консоли (если применимо)
    ULONG CountX;                      // +0x054: Ширина окна консоли в символах
    ULONG CountY;                      // +0x058: Высота окна консоли в символах
    ULONG CountCharsX;                 // +0x05C: Ширина буфера консоли в символах
    ULONG CountCharsY;                 // +0x060: Высота буфера консоли в символах
    ULONG FillAttribute;               // +0x064: Атрибуты консоли (цвет фона и текста)
    ULONG WindowFlags;                 // +0x068: Флаги, определяющие состояние окна консоли
    ULONG ShowWindowFlags;             // +0x06C: Флаги для показа окна
    UNICODE_STRING WindowTitle;        // +0x070: Заголовок окна
    UNICODE_STRING DesktopInfo;        // +0x078: Информация о рабочем столе (имя десктопа)
    UNICODE_STRING ShellInfo;          // +0x080: Информация о оболочке (обычно `cmd.exe`)
    UNICODE_STRING RuntimeData;        // +0x088: Данные, используемые во время выполнения
    PVOID CurrentDirectories[32];      // +0x090: Массив текущих каталогов для каждого диска
    ULONG EnvironmentSize;             // +0x0B0: Размер переменных окружения
    ULONG EnvironmentVersion;          // +0x0B4: Версия среды
    PVOID PackageDependencyData;       // +0x0B8: Данные зависимости пакета
    ULONG ProcessGroupId;              // +0x0BC: Идентификатор группы процессов
    ULONG LoaderThreads;               // +0x0C0: Количество потоков загрузчика
    UNICODE_STRING RedirectionDllName; // +0x0C8: Имя DLL для редиректа (если используется)
    UNICODE_STRING HeapPartitionName;  // +0x0D0: Имя раздела кучи
    ULONG64 DefaultThreadpoolCpuSetMasks; // +0x0D8: Маски CPU для потоков
    ULONG DefaultThreadpoolCpuSetMaskCount; // +0x0E0: Количество масок CPU для потоков
} RTL_USER_PROCESS_PARAMETERS, * PRTL_USER_PROCESS_PARAMETERS;


typedef struct _PEB {
    BOOLEAN InheritedAddressSpace;   // +0x000: Указывает, был ли адресный режим унаследован от родительского процесса
    BOOLEAN ReadImageFileExecOptions; // +0x001: Указывает, были ли прочитаны параметры выполнения образа
    BOOLEAN BeingDebugged;           // +0x002: Указывает, выполняется ли процесс под отладчиком
    BOOLEAN SpareBool;               // +0x003: Зарезервированное поле
    HANDLE Mutant;                   // +0x004: Указатель на объект-событие синхронизации
    PVOID ImageBaseAddress;          // +0x008: Базовый адрес образа процесса
    PPEB_LDR_DATA Ldr;               // +0x00C: Указатель на структуру загрузчика модулей (PEB_LDR_DATA)
    PRTL_USER_PROCESS_PARAMETERS ProcessParameters; // +0x010: Указатель на параметры процесса (например, переменные среды, командную строку)
    PVOID SubSystemData;             // +0x014: Указатель на данные подсистемы
    PVOID ProcessHeap;               // +0x018: Указатель на основной кучевой сегмент процесса
    PVOID FastPebLock;               // +0x01C: Указатель на объект синхронизации PEB
    PVOID FastPebLockRoutine;        // +0x020: Указатель на функцию блокировки PEB
    PVOID FastPebUnlockRoutine;      // +0x024: Указатель на функцию разблокировки PEB
    ULONG EnvironmentUpdateCount;    // +0x028: Счетчик обновлений переменных среды
    PVOID KernelCallbackTable;       // +0x02C: Указатель на таблицу обратных вызовов ядра
    ULONG SystemReserved[1];         // +0x030: Зарезервированные данные
    ULONG AtlThunkSListPtr32;        // +0x034: Зарезервировано
    PVOID FreeList;                  // +0x038: Зарезервировано
    ULONG TlsExpansionCounter;       // +0x03C: Счетчик расширений TLS
    PVOID TlsBitmap;                 // +0x040: Указатель на карту битов TLS
    ULONG TlsBitmapBits[2];          // +0x044: Биты карты TLS
    PVOID ReadOnlySharedMemoryBase;  // +0x04C: Указатель на базу памяти, доступной только для чтения
    PVOID ReadOnlySharedMemoryHeap;  // +0x050: Указатель на кучу для памяти только для чтения
    PVOID ReadOnlyStaticServerData;  // +0x054: Указатель на данные сервера
    PVOID AnsiCodePageData;          // +0x058: Указатель на данные кодовой страницы ANSI
    PVOID OemCodePageData;           // +0x05C: Указатель на данные OEM кодовой страницы
    PVOID UnicodeCaseTableData;      // +0x060: Указатель на таблицу преобразования регистра Unicode
    ULONG NumberOfProcessors;        // +0x064: Количество процессоров
    ULONG NtGlobalFlag;              // +0x068: Глобальные флаги NT, установленные для процесса
    LARGE_INTEGER CriticalSectionTimeout; // +0x070: Время ожидания для критических секций
    ULONG HeapSegmentReserve;        // +0x078: Зарезервированный размер сегмента кучи
    ULONG HeapSegmentCommit;         // +0x07C: Коммитированный размер сегмента кучи
    ULONG HeapDeCommitTotalFreeThreshold; // +0x080: Порог деактивации свободной памяти в куче
    ULONG HeapDeCommitFreeBlockThreshold; // +0x084: Порог деактивации свободных блоков в куче
    ULONG NumberOfHeaps;             // +0x088: Количество куч в процессе
    ULONG MaximumNumberOfHeaps;      // +0x08C: Максимальное количество куч в процессе
    PVOID* ProcessHeaps;             // +0x090: Указатель на массив указателей на кучи процесса
    PVOID GdiSharedHandleTable;      // +0x094: Указатель на таблицу GDI
    PVOID ProcessStarterHelper;      // +0x098: Указатель на помощника запуска процесса
    ULONG GdiDCAttributeList;        // +0x09C: Атрибуты GDI
    PVOID LoaderLock;                // +0x0A0: Блокировка загрузчика модулей
    ULONG OSMajorVersion;            // +0x0A4: Основная версия операционной системы
    ULONG OSMinorVersion;            // +0x0A8: Минорная версия операционной системы
    ULONG OSBuildNumber;             // +0x0AC: Номер сборки операционной системы
    ULONG OSPlatformId;              // +0x0B0: Идентификатор платформы
    ULONG ImageSubsystem;            // +0x0B4: Подсистема, в которой работает образ
    ULONG ImageSubsystemMajorVersion; // +0x0B8: Основная версия подсистемы
    ULONG ImageSubsystemMinorVersion; // +0x0BC: Минорная версия подсистемы
    ULONG ActiveProcessAffinityMask; // +0x0C0: Маска аффинити активного процесса
    ULONG GdiHandleBuffer[34];       // +0x0C4: Буфер для хендлов GDI
    PVOID PostProcessInitRoutine;    // +0x14C: Указатель на функцию инициализации после запуска процесса
    PVOID TlsExpansionBitmap;        // +0x150: Указатель на расширение битовой карты TLS
    ULONG TlsExpansionBitmapBits[32]; // +0x154: Биты расширенной карты TLS
    ULONG SessionId;                 // +0x1D4: Идентификатор сеанса
} PEB, * PPEB;