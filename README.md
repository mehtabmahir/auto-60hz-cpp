# Fullscreen Mode Refresh Rate Controller

![alt text](https://github.com/mehtabmahir/auto-60hz-cpp/blob/main/preview.png "Preview")

To compile: Clone and then use Qt Creator, open the CMakeLists.txt and run the build (Ctrl+B).

This application dynamically adjusts the screen's refresh rate based on whether a window is in fullscreen mode or not. The program monitors the active window and modifies the display refresh rate to either 60Hz or 120Hz to optimize performance and conserve battery life, especially on laptops or portable devices.

## Features
- **Qt-Based User Interface**: The UI has been overhauled using **Qt** for a more modern, user-friendly experience.
- **Run on Startup**: Added toggle for run on startup.
- **Fullscreen Detection**: Automatically detects when an application switches to fullscreen mode.
- **Refresh Rate Control**: Switches the refresh rate between 60Hz and 120Hz depending on the window's state.
- **System Process Filtering**: Ignores system applications such as `explorer.exe`, `widgets.exe`, and `snippingtool.exe` to focus only on user applications.
- **Low Resource Usage**: The program is optimized to minimize CPU usage (<0.1%), ensuring your system's power usage remains unaffected while it runs.
- **Checks for AC**: Skips the script if the device is plugged into power.

## Efficiency
The program is designed to run in the background with minimal CPU usage (<0.1%). By reducing unnecessary screen refresh rates when not in fullscreen mode, it can help improve battery life by up to **20%** on portable devices.

## Key Use Cases
- **Watching Videos**: For applications like YouTube or media players, the program lowers the refresh rate to 60Hz while in fullscreen mode, conserving battery life without impacting video playback quality. (Since most videos are not 120Hz anyways.)
- **General Use**: You can enjoy the smoothness of 120Hz the rest of the time.

## How It Works
1. **Fullscreen Mode Detection**: The application continuously monitors the active window to check if it is in fullscreen mode.
2. **Refresh Rate Adjustment**: If the window is in fullscreen mode, the refresh rate is set to **60Hz**. If not, it sets the refresh rate to **120Hz**. (User adjustable)
3. **System Processes Exclusion**: The program excludes common system processes that shouldn't trigger a refresh rate change.

## APIs, Frameworks, and Libraries Used
- **Qt Framework**: The new UI is implemented in Qt, providing an intuitive and visually appealing interface.
- **Windows API**: Used for interacting with system-level functions such as window handling and display settings.
- **PSAPI**: The `psapi.h` library is used to retrieve process information like the process name associated with a window.
- **DEVMODE**: The `DEVMODE` structure is used to retrieve and change display settings, including refresh rate.
- **Algorithm and Vector Libraries**: `<algorithm>` and vectors are used for filtering and processing application names.

## Installation
To run this program, you need a Windows environment with access to the `Windows API` and **Qt runtime** for the GUI. Simply compile and execute the program with any C++ compiler that supports Qt.

### Prerequisites
- Windows OS
- Qt development environment installed (for custom compilation)
- Any C++ compiler supporting Windows API and Qt.

### Compilation
Clone and then use Qt Creator, open the CMakeLists.txt and run the build (Ctrl+B).
Alternatively, you can download the precompiled `.exe` file from the release page and run it directly.

## How to Use
1. Go to the directory containing the source code and compile it with your configured C++ compiler and Qt environment, or download the precompiled `.exe` file from the [release page](#).
2. Run the compiled executable.
3. Use the Qt-based UI to:
   - Configure custom refresh rates for fullscreen and non-fullscreen modes.
   - Enable or disable the "Run on Startup" feature.
   - Monitor the program's status in real-time.
4. The program will continuously check the fullscreen state of active applications and adjust the refresh rate as necessary.

## Future Updates
I am actively working on improving this tool and plan to introduce more features in upcoming updates, including:
- **UI Improvements**: Ongoing improvements to the UI for better usability and aesthetics. ✓ Implemented
- **Configurable refresh rates**: ✓ Implemented
- **Run on startup**: ✓ Implemented
- **Run hidden in tray**: ✓ Fixed
- **Hibernate mode issue**: ✓ Fixed

Stay tuned for new releases!

## Limitations/Issues
- This program only works on Windows operating systems.
- Requires the Qt runtime environment for proper execution.

## License
This project is licensed under the MIT License. See the full license text below:

```
MIT License

Copyright (c) [2025] [Mehtab Mahir]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

## Contributing
Feel free to fork and contribute to this project! Pull requests for improvements, optimizations, or bug fixes are welcome.

