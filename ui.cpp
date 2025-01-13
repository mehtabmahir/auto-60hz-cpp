#include <windows.h>
#include <string>
#include <fstream>
#include <iostream>
#include <shlwapi.h>  // For registry functions
#include <filesystem>
#include "main.cpp"

// Declare the global text box handles
HWND hInputHigh, hInputLow, hStartupCheckbox;

// Forward declarations of functions
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void ProcessInputValues(HWND hWnd);
void SaveValuesToFile(int high, int low);
bool LoadValuesFromFile(int &high, int &low);
void LoadAndPopulateValues(HWND hWnd);
void SetStartup(bool enable);
bool IsStartupEnabled();

// Entry point
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"Sample Window Class";
    
    // Register the window class
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);
    
    // Create the window
    HWND hWnd = CreateWindowEx(0, CLASS_NAME, L"High and Low Input", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 250, nullptr, nullptr, hInstance, nullptr);
    
    if (hWnd == nullptr) {
        return 0;
    }

    // Try to load saved values
    LoadAndPopulateValues(hWnd);

    // Show the window
    ShowWindow(hWnd, nCmdShow);
    
    // Main message loop
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int) msg.wParam;
}

// Window procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        // Create input fields for high and low values
        CreateWindow(L"STATIC", L"Enter High Value:", WS_VISIBLE | WS_CHILD, 10, 20, 150, 20, hWnd, nullptr, nullptr, nullptr);
        hInputHigh = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 160, 20, 100, 20, hWnd, nullptr, nullptr, nullptr);
        
        CreateWindow(L"STATIC", L"Enter Low Value:", WS_VISIBLE | WS_CHILD, 10, 60, 150, 20, hWnd, nullptr, nullptr, nullptr);
        hInputLow = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 160, 60, 100, 20, hWnd, nullptr, nullptr, nullptr);
        
        // Add a button to process the input
        CreateWindow(L"BUTTON", L"Submit", WS_VISIBLE | WS_CHILD, 100, 100, 100, 30, hWnd, (HMENU)1, nullptr, nullptr);
        
        // Add a checkbox for "Run on Startup"
        hStartupCheckbox = CreateWindow(L"BUTTON", L"Run on Startup", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 10, 140, 200, 20, hWnd, (HMENU)2, nullptr, nullptr);
        
        // Check the checkbox if startup is enabled
        if (IsStartupEnabled()) {
            SendMessage(hStartupCheckbox, BM_SETCHECK, BST_CHECKED, 0);
        }
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == 1) {
            // When the button is clicked, process the input
            ProcessInputValues(hWnd);
        } else if (LOWORD(wParam) == 2) {
            // Handle the "Run on Startup" checkbox
            LRESULT isChecked = SendMessage(hStartupCheckbox, BM_GETCHECK, 0, 0);
            SetStartup(isChecked == BST_CHECKED);
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return 0;
}

// Function to process input values
void ProcessInputValues(HWND hWnd) {
    // Retrieve the text from the textboxes
    int highLength = GetWindowTextLength(hInputHigh) + 1;
    int lowLength = GetWindowTextLength(hInputLow) + 1;
    
    char* highValue = new char[highLength];
    char* lowValue = new char[lowLength];

    GetWindowTextA(hInputHigh, highValue, highLength);
    GetWindowTextA(hInputLow, lowValue, lowLength);

    // Convert the values to integers
    int high = std::stoi(std::string(highValue));
    int low = std::stoi(std::string(lowValue));

    // Pass the values to the other file (you can call another function here)
    SetHighLowValues(high, low);    

    // Save the values to file for future use
    SaveValuesToFile(high, low);

    // Signal the old thread to stop by setting the flag
    shouldStop.store(true);  // Set the atomic flag to true

    // If the old thread is running, wait for it to finish
    if (logicThread.joinable()) {
        logicThread.join();  // Wait for the old thread to finish
    }

    // Reset the stop flag to allow the new thread to run
    shouldStop.store(false);  

    // Start a new thread for mainScript
    logicThread = std::thread(mainScript);  // Start the new thread
    logicThread.detach();  // Detach it so it runs independently

    // Clean up
    delete[] highValue;
    delete[] lowValue;
}

// Function to save the high and low values to a file
void SaveValuesToFile(int high, int low) {
    // Get the current directory (where the executable is located)
    wchar_t exePath[MAX_PATH];
    GetModuleFileName(nullptr, exePath, MAX_PATH);
    
    // Extract the directory part of the executable path
    PathRemoveFileSpec(exePath);  // Removes the file part, leaving only the directory path

    // Create the full path for settings.txt
    std::wstring settingsFilePath = std::wstring(exePath) + L"\\settings.txt";

    // Convert the wide string to a narrow string
    char narrowPath[MAX_PATH];
    wcstombs(narrowPath, settingsFilePath.c_str(), MAX_PATH);

    // Open the file in write mode, and if it exists, overwrite it.
    std::ofstream outFile(narrowPath, std::ios::out | std::ios::trunc);

    // Check if the file was successfully opened
    if (outFile.is_open()) {
        // Write the values to the file
        outFile << high << std::endl;
        outFile << low << std::endl;

        // Close the file
        outFile.close();
    } else {
        MessageBox(nullptr, L"Error opening file for saving values. Ensure the program has write permissions.", L"File Error", MB_OK | MB_ICONERROR);
    }
}

// Function to read the high and low values from the file
bool LoadValuesFromFile(int &high, int &low) {
    // Get the current directory (where the executable is located)
    wchar_t exePath[MAX_PATH];
    GetModuleFileName(nullptr, exePath, MAX_PATH);
    
    // Extract the directory part of the executable path
    PathRemoveFileSpec(exePath);  // Removes the file part, leaving only the directory path

    // Create the full path for settings.txt
    std::wstring settingsFilePath = std::wstring(exePath) + L"\\settings.txt";

    // Convert the wide string to a narrow string
    char narrowPath[MAX_PATH];
    wcstombs(narrowPath, settingsFilePath.c_str(), MAX_PATH);

    std::ifstream inFile(narrowPath);
    if (inFile.is_open()) {
        inFile >> high;
        inFile >> low;
        inFile.close();
        return true;  // Successfully loaded values
    } else {
        MessageBox(nullptr, L"Error opening file for loading values. Make sure settings.txt exists", L"File Error", MB_OK | MB_ICONERROR);
        return false;  // File not found or read error
    }
}


// Function to load and populate the textboxes with saved values
void LoadAndPopulateValues(HWND hWnd) {
    int high = 0, low = 0;
    if (LoadValuesFromFile(high, low)) {
        // Populate the textboxes with loaded values if available
        SetWindowTextA(hInputHigh, std::to_string(high).c_str());
        SetWindowTextA(hInputLow, std::to_string(low).c_str());
        ProcessInputValues(hWnd);
    }
}

// Function to set the application to run on startup
void SetStartup(bool enable) {
    HKEY hKey;
    const wchar_t *subKey = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    const wchar_t *valueName = L"MyApp";
    wchar_t exePath[MAX_PATH];
    GetModuleFileName(nullptr, exePath, MAX_PATH);

    if (RegOpenKeyEx(HKEY_CURRENT_USER, subKey, 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        if (enable) {
            RegSetValueEx(hKey, valueName, 0, REG_SZ, (const BYTE*)exePath, (DWORD)((wcslen(exePath) + 1) * sizeof(wchar_t)));
        } else {
            RegDeleteValue(hKey, valueName);
        }
        RegCloseKey(hKey);
    }
}

// Function to check if the application is set to run on startup
bool IsStartupEnabled() {
    HKEY hKey;
    const wchar_t *subKey = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    const wchar_t *valueName = L"MyApp";
    wchar_t exePath[MAX_PATH];
    GetModuleFileName(nullptr, exePath, MAX_PATH);

    if (RegOpenKeyEx(HKEY_CURRENT_USER, subKey, 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS) {
        wchar_t value[MAX_PATH];
        DWORD size = sizeof(value);
        bool isEnabled = RegQueryValueEx(hKey, valueName, nullptr, nullptr, (LPBYTE)value, &size) == ERROR_SUCCESS &&
                         wcscmp(value, exePath) == 0;
        RegCloseKey(hKey);
        return isEnabled;
    }
    return false;
}
