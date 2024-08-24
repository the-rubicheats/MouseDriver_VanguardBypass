#include <Windows.h>
#include <iostream>

#define IOCTL_MOUSE_MOVE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x696, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

struct Request {
    long x;
    long y;
    unsigned short button_flags;
};

void sendMouseMove(HANDLE hDevice, long x, long y, unsigned short button_flags) {
    Request request = { x, y, button_flags };
    DWORD returned = 0;

    BOOL result = DeviceIoControl(
        hDevice,
        IOCTL_MOUSE_MOVE,
        &request,
        sizeof(Request),
        &request,
        sizeof(Request),
        &returned,
        NULL
    );

    if (!result) {
        std::cerr << "Error: " << GetLastError() << std::endl;
    }
    else {
        std::cout << "Mouse moved successfully.\n";
    }
}

int main() {
    HANDLE hDevice = CreateFile(
        L"\\\\.\\UC", 
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );

    if (hDevice == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open device: " << GetLastError() << std::endl;
        return 1;
    }

    std::cout << "Device opened successfully.\n";

    sendMouseMove(hDevice, 100, 100, 0);


    sendMouseMove(hDevice, 0, 0, 0x01); 
    sendMouseMove(hDevice, 0, 0, 0x02); 

    CloseHandle(hDevice);
    for (;;);
    return 0;
}
