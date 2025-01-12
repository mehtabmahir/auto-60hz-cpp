# Fullscreen Mode Refresh Rate Controller

This application dynamically adjusts the screen's refresh rate based on whether a window is in fullscreen mode or not. The program monitors the active window and modifies the display refresh rate to either 60Hz or 120Hz to optimize performance and conserve battery life, especially on laptops or portable devices.

## Features
- **Fullscreen Detection**: Automatically detects when an application switches to fullscreen mode.
- **Refresh Rate Control**: Switches the refresh rate between 60Hz and 120Hz depending on the window's state.
- **System Process Filtering**: Ignores system applications such as `explorer.exe`, `widgets.exe`, and `snippingtool.exe` to focus only on user applications.
- **Low Resource Usage**: The program is optimized to minimize CPU usage (<0.1%), ensuring your system's performance remains unaffected while it runs.

## Efficiency
The program is designed to run in the background with minimal CPU usage (<0.1%). By reducing unnecessary screen refresh rates when not in fullscreen mode, it can help improve battery life by up to **20%** on portable devices.

## Key Use Cases
- **Watching Videos**: For applications like YouTube or media players, the program lowers the refresh rate to 60Hz while in fullscreen mode, conserving battery life without impacting video playback quality. (Since most videos are not 120hz anyways)
- **General Use**: You can enjoy the smoothness of 120hz the rest of the time.
## How It Works
1. **Fullscreen Mode Detection**: The application continuously monitors the active window to check if it is in fullscreen mode.
2. **Refresh Rate Adjustment**: If the window is in fullscreen mode, the refresh rate is set to **60Hz**. If not, it sets the refresh rate to **120Hz**.
3. **System Processes Exclusion**: The program excludes common system processes that shouldn't trigger a refresh rate change.

## APIs and Libraries Used
- **Windows API**: The program uses the `Windows.h` header for interacting with system-level functions such as window handling and display settings.
- **PSAPI**: The `psapi.h` library is used to retrieve process information like the process name associated with a window.
- **DEVMODE**: The `DEVMODE` structure is used to retrieve and change display settings, including refresh rate.
- **Algorithm and Vector Libraries**: `<algorithm>` and vectors are used for filtering and processing application names.

## Installation
To run this program, you need a Windows environment with access to the `Windows API`. Simply compile and execute the program  with any C++ compiler.

Alternatively, you can download the precompiled `.exe` file from the and run it directly.

### Prerequisites
- Windows OS
- Any C++ compiler supporting Windows API.

## How to Use
1. Open the source code in VS Code and compile it with your configured C++ compiler, or download the `.exe` file from the [release page](#).
2. Run the compiled executable.
3. The program will continuously check the fullscreen state of active applications and adjust the refresh rate as necessary.

## Future Updates
I am actively working on improving this tool and plan to introduce more features in upcoming updates, including:
- Configurable refresh rates.
- User interface
- Run on startup (You can do it manually by putting it in the Startup folder)
Stay tuned for new releases!
- Run hidden on tray

## Limitations
- This program only works on Windows operating systems.
- The refresh rate adjustments are set to 60Hz and 120Hz; modifying other refresh rates is not supported. (yet)

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

---
