# Window Border Color

Set the border color of all top-level window for Windows 11 Build 22000 and later.
![](/screenshot/after.jpg)

## Description

When windows uses dark mode, the border of the overlapped window is not clear, which can be alleviated by setting the windows theme color to some distinguishable colors and applying it to the title bar and border  
The screenshot below shows some of the issues when using red as the theme color:
![](/screenshot/before.jpg)

-   Only the top-most window uses the theme color as the border color; other windows have a gray border color, which is difficult to distinguish in dark mode.
-   Many of top-most window don't adhere to the theme color, including electron based apps such as Visual Studio Code and so on.

This program helps to mitigating these issues by hooks all top-level window and calls DwmSetWindowAttribute with DWMWA_BORDER_COLOR(which only support Windows 11 Build 22000 and later) when the window switches between active and inactive

Hopefully Microsoft will improve these issues in the future and this program will no longer be needed

## Usage

Install [visual c++ 2022 runtime](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170) both for x86 and x64 before first-time run.

Run WinBorderColor_x86.exe and WinBorderColor_x64.exe both as administrator[^1].

If you want to run it automatically when windows starts up, create a task in the Task Scheduler for each program and select the following two options:

-   Run only when user is logged on
-   Run with highest privileges[^1]

> [^1] Some apps, such as Task Manager, require administrator/highest privileges.

## Config

You can customize the colors of the forground and background window by editing the WinBorderColor.ini file.  
The color format is 0x00bbggrr. 0xFFFFFFFE means no border. 0xFFFFFFFF means the default color of the system.

## License

This project is licensed under the MIT License - see the LICENSE file for details
