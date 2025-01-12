#include <windows.h>
#include <string>
#include <fstream>
#include <iostream>
#include "main.cpp"

// Declare the global text box handles
HWND hInputHigh, hInputLow;

// Forward declarations of functions
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void ProcessInputValues(HWND hWnd);
void SaveValuesToFile(int high, int low);
bool LoadValuesFromFile(int &high, int &low);
void LoadAndPopulateValues(HWND hWnd);

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
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 200, nullptr, nullptr, hInstance, nullptr);
    
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
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == 1) {
            // When the button is clicked, process the input
            ProcessInputValues(hWnd);
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
    // Open the file in write mode, and if it exists, overwrite it.
    std::ofstream outFile("settings.txt", std::ios::out | std::ios::trunc);

    // Check if the file was successfully opened
    if (outFile.is_open()) {
        // Write the values to the file
        outFile << high << std::endl;
        outFile << low << std::endl;

        // Close the file
        outFile.close();

        // Confirm that the values were saved
        MessageBox(nullptr, L"Values saved to settings.txt", L"File Saved", MB_OK | MB_ICONINFORMATION);
    } else {
        MessageBox(nullptr, L"Error opening file for saving values. Ensure the program has write permissions.", L"File Error", MB_OK | MB_ICONERROR);

        // Debugging output: get the current directory using _wgetcwd (wide-char)
        wchar_t currentDir[MAX_PATH];
        _wgetcwd(currentDir, MAX_PATH);  // Use wide-character version

        // Output current directory (wchar_t to char* conversion for displaying in console)
        char dirBuffer[MAX_PATH];
        wcstombs(dirBuffer, currentDir, MAX_PATH);  // Convert wchar_t to char* for console output
        std::cout << "Current directory: " << dirBuffer << std::endl;  // Print current working directory
    }
}

// Function to read the high and low values from the file
bool LoadValuesFromFile(int &high, int &low) {
    std::ifstream inFile("settings.txt");
    if (inFile.is_open()) {
        inFile >> high;
        inFile >> low;
        inFile.close();
        return true;  // Successfully loaded values
    } else {
        MessageBox(nullptr, L"Error opening file for loading values.", L"File Error", MB_OK | MB_ICONERROR);
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
