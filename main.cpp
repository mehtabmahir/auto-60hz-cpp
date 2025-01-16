#include <windows.h>
#include <iostream>
#include <psapi.h>
#include <vector>
#include <algorithm>
#include <Windows.h>
#include <thread>
#include <atomic>

std::atomic<bool> shouldStop(false);  // Atomic flag to control thread stop
std::thread logicThread;  // Global thread object
int currentRefreshRate, upperHz, lowerHz; 

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

bool CheckFullscreenState() {
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
                    trackedHwnd = hwnd;
                    isFullscreen = true;
                }
            } else if (hwnd == trackedHwnd && !IsWindowFullscreen(hwnd)) {
                trackedHwnd = nullptr;
                isFullscreen = false;
            }
        } 
    } 
    else {
        if (trackedHwnd && !IsWindowFullscreen(trackedHwnd)) {
            trackedHwnd = nullptr;
            isFullscreen = false;
        }
    }

    return isFullscreen;
}
// function to set upper and lower values
void SetHighLowValues(int high, int low) {
    upperHz = high;
    lowerHz = low;
}
// function to change refresh rate
void changeRefreshRate(DEVMODE devMode, int target) {
    devMode.dmDisplayFrequency = target;
    if (currentRefreshRate != target) {
        if (ChangeDisplaySettings(&devMode, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL) {
            currentRefreshRate = target;
        }
    }
}

int mainScript() {
    DEVMODE devMode;
    devMode.dmSize = sizeof(DEVMODE);
    devMode.dmDriverExtra = 0;

    // Get current settings
    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode);
    // Get AC status
    SYSTEM_POWER_STATUS batteryStatus;


    while (!shouldStop) {
        GetSystemPowerStatus(&batteryStatus); // update battery status
        if (batteryStatus.ACLineStatus == 0) { // check if device is not plugged in
            if (CheckFullscreenState()) // lower refresh rate if fullscreen
                changeRefreshRate(devMode, lowerHz);
            else // increase refresh rate if not fullscreen
                changeRefreshRate(devMode, upperHz);
        }
        else if (currentRefreshRate == lowerHz) // switches to upper refresh rate otherwise if needed
            changeRefreshRate(devMode, upperHz); 
        Sleep(1000); // Check every second
    }
    return 0;
}
