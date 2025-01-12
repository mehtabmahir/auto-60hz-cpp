#include <windows.h>
#include <iostream>
#include <psapi.h>
#include <vector>
#include <algorithm>
#include <Windows.h>



// Function to determine if a window is in fullscreen mode
bool IsWindowFullscreen(HWND hwnd) {
    RECT windowRect;
    GetWindowRect(hwnd, &windowRect);

    // Ignore windows with dimensions (0, 0) as they indicate the window is in the background
    if (windowRect.left == 0 && windowRect.right == 0 && windowRect.top == 0 && windowRect.bottom == 0) {
        return true;
    }

    // Get screen width and height
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Check if the window covers the entire screen or is in the background (0, 0)
    return (windowRect.left == 0 && windowRect.top == 0 && windowRect.right == screenWidth && windowRect.bottom == screenHeight);
}

// Function to get the name of the application running in a window
std::string GetProcessName(HWND hwnd) {
    DWORD processId;
    GetWindowThreadProcessId(hwnd, &processId);
    HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
    if (processHandle) {
        char processName[MAX_PATH];
        if (GetModuleBaseNameA(processHandle, nullptr, processName, sizeof(processName))) {
            CloseHandle(processHandle);
            return std::string(processName);
        }
        CloseHandle(processHandle);
    }
    return "<Unknown>";
}

// List of known system processes to ignore
const std::vector<std::string> systemProcesses = {
    "explorer.exe",
    "widgets.exe",
    "snippingtool.exe"
};

BOOL CheckFullscreenState() {
    static HWND trackedHwnd = nullptr;
    static bool isFullscreen = false;

    HWND hwnd = GetForegroundWindow();
    if (hwnd) {
        std::string appName = GetProcessName(hwnd);

        // Convert the process name to lowercase for comparison
        std::transform(appName.begin(), appName.end(), appName.begin(), ::tolower);

        // Check if the application is not a system process
        if (std::find(systemProcesses.begin(), systemProcesses.end(), appName) == systemProcesses.end()) {
            if (IsWindowFullscreen(hwnd)) {
                if (!isFullscreen) {
                    std::cout << "The active window is in fullscreen mode: " << appName << std::endl;
                    trackedHwnd = hwnd;
                    isFullscreen = true;
                }
            } else if (hwnd == trackedHwnd && !IsWindowFullscreen(hwnd)) {
                std::cout << "The active window has exited fullscreen mode: " << appName << std::endl;
                trackedHwnd = nullptr;
                isFullscreen = false;
            }
        } else {
            std::cout << "Ignoring system process: " << appName << std::endl;
        }
    } else {
        if (trackedHwnd && !IsWindowFullscreen(trackedHwnd)) {
            std::string appName = GetProcessName(trackedHwnd);
            std::cout << "The tracked window has exited fullscreen mode: " << appName << std::endl;
            trackedHwnd = nullptr;
            isFullscreen = false;
        }
    }

    return isFullscreen;

    
}

int main() {
    DEVMODE devMode;
    devMode.dmSize = sizeof(DEVMODE);
    devMode.dmDriverExtra = 0;
    int currentRefreshRate;


    if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode) == 0) {
        std::cerr << "Error getting display settings." << std::endl;
        return 1;
    }


    std::cout << "Checking fullscreen application state...\n";
    while (true) {
        
        if (CheckFullscreenState()) {
            devMode.dmDisplayFrequency = 60;
            if (currentRefreshRate != 60){
                if (ChangeDisplaySettings(&devMode, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL) {
                    std::cout << "Refresh rate set to 60hz" << std::endl;
                    currentRefreshRate = 60;
                }
                else
                    std::cerr << "Error changing refresh rate." << std::endl;
            }
        }
        else {
            devMode.dmDisplayFrequency = 120;
            if (currentRefreshRate != 120) {
                if (ChangeDisplaySettings(&devMode, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL) {
                    std::cout << "Refresh rate set to 120hz" << std::endl;
                    currentRefreshRate = 120;
                }
                else
                    std::cerr << "Error changing refresh rate." << std::endl;
            }
        }
        Sleep(1000); // Check every second
    }
    return 0;
}
