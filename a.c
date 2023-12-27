#include <iostream>
#include <windows.h>
typedef unsigned long long QWORD;
// Define your IOCTL codes
#define IOCTL_CODES_COUNT 8
#define IOCTL_CODES_ARRAY { 0x00094264, 0x00098268, 0x00170008, 0x00170010, 0x00170034, 0x001700ac, 0x0017080c, 0x00170810 }

// Define buffer sizes for each IOCTL
#define BUFFER_SIZES { 8192, 8192, 8192, 8192, 8192, 8192, 8192, 8192 }

int main() {
    // Step 1: Open a handle to your driver
    HANDLE hDevice = CreateFile(L"\\\\.\\NDIS", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hDevice == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open driver: " << GetLastError() << std::endl;
        return 1;
    }

    // Step 2: Prepare the IOCTL input and output buffers with the correct sizes
    ULONGLONG inputData[IOCTL_CODES_COUNT];
    ULONGLONG outputData[IOCTL_CODES_COUNT];
    DWORD bufferSizes[IOCTL_CODES_COUNT] = BUFFER_SIZES;

    inputData[0] = 0x1122334455667788; // Sample 8-byte data for IOCTL 0x00094264
    inputData[1] = 0xAABBCCDDEEFF0011; // Sample 8-byte data for IOCTL 0x00098268
    inputData[2] = 0x2233445566778899; // Sample 8-byte data for IOCTL 0x00170008
    inputData[3] = 0x33445566778899AA; // Sample 8-byte data for IOCTL 0x00170010
    inputData[4] = 0x445566778899AABB; // Sample 8-byte data for IOCTL 0x00170034
    inputData[5] = 0x5566778899AABBCC; // Sample 8-byte data for IOCTL 0x001700ac
    inputData[6] = 0x66778899AABBCCDD; // Sample 8-byte data for IOCTL 0x0017080c
    inputData[7] = 0x778899AABBCCDDEE; // Sample 8-byte data for IOCTL 0x00170810

    // Step 3: Loop through and send requests for all IOCTL codes
    DWORD bytesReturned;
    DWORD ioctlCodes[IOCTL_CODES_COUNT] = IOCTL_CODES_ARRAY;

    for (int i = 0; i < IOCTL_CODES_COUNT; i++) {
        std::cout << "Sending IOCTL request with code: " << std::hex << ioctlCodes[i] << std::dec << std::endl;

        // Send IOCTL request with the correct buffer sizes
        if (!DeviceIoControl(hDevice, ioctlCodes[i], &inputData[i], sizeof(inputData[i]), &outputData[i], bufferSizes[i], &bytesReturned, NULL)) {
            std::cerr << "Failed to send IOCTL request: " << GetLastError() << std::endl;
        }
        else {
            std::cout << "IOCTL request sent successfully. Output data: " << std::hex << outputData[i] << std::dec << std::endl;
        }

        // Sleep or perform other actions as needed between IOCTL requests
        Sleep(1000); // Adjust the sleep duration as needed
    }

    // Step 4: Close the handle to the driver
    CloseHandle(hDevice);

    return 0;
}
