# Window Border Color

Set the border color of all top-level window for Windows 11 Build 22000 and later.

## Description

When windows uses dark mode, the border of the overlapped window is not clear, which can be alleviated by setting the windows theme color to some distinguishable colors and applying it to the title bar and border, but many window don't adhere to theme colors, especially in electron based apps including Visual Studio Code.

This program helps to solve this problem.  
This program hooks all top-level window and calls DwmSetWindowAttribute with DWMWA_BORDER_COLOR when the window switch between active and inactive

Hopefully Microsoft will improve this issue in the future and this program will no longer be needed

## Usage

Just run WinBorderColor32.exe and WinBorderColor64.exe both as administrator[^1].

If you want to run it automatically when windows starts up, create a task in the Task Scheduler for each program and select the following two options:

-   Run only when user is logged on
-   Run with highest privileges[^1]

> [^1] Some apps, such as Task Manager, require administrator/highest privileges.

## License

This project is licensed under the [NAME HERE] License - see the LICENSE.md file for details
