// Made by Revolex & ThichQuangDuc.

#include <iostream>
#include <Windows.h>

//#define NF_MOUSE_EVENT CTL_CODE(0x617F, 0x18B, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define NF_MOUSE_EVENT 0x617F862C
//#define NF_FAKE_U_DATA CTL_CODE(0x8F67, 0x5EC, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define NF_FAKE_AUTH_DATA 0x8F67D7B0

typedef struct _NF_MOUSE_REQUEST {
	long x;
	long y;
	unsigned short button_flags;
}NF_MOUSE_REQUEST, * PNF_MOUSE_REQUEST;

class driver_control {
private:

	HANDLE driver_handle = 0;

public:

	driver_control(const char* path);

	~driver_control();

	void Mouse_Move(long x, long y, unsigned short button_flags);
	void Fake_Auth_Data(long data);
};

#define IS_VALID_HANDLE(handle)				(handle && handle != INVALID_HANDLE_VALUE)

driver_control::driver_control(const char* path) {
	this->driver_handle = CreateFileA(path, GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (IS_VALID_HANDLE(this->driver_handle)) {
			printf("Connected.\n");
		}
}

driver_control::~driver_control() {
	CloseHandle(this->driver_handle);
}

void driver_control::Mouse_Move(long x, long y, unsigned short button_flags) {
	if (this->driver_handle) {
		ULONG bytes;
		NF_MOUSE_REQUEST mouse_request = { 0 };
		mouse_request.x = x;
		mouse_request.y = y;
		mouse_request.button_flags = button_flags;
		BOOL success = DeviceIoControl(this->driver_handle, NF_MOUSE_EVENT, &mouse_request, sizeof(mouse_request), &mouse_request, sizeof(mouse_request), &bytes, 0);

		if (!success)
			std::cerr << "Mouse_Move DeviceIoControl failed!. Error Code: " << GetLastError() << std::endl;
		else
			std::cout << "Mouse_Move DeviceIoControl success." << std::endl;
	}
}

bool SetSystemTimeToSpecificTime() {
	SYSTEMTIME st;
	GetSystemTime(&st);

	st.wYear = 2024;
	st.wMonth = 1;
	st.wDay = 1;
	st.wHour = 12;
	st.wMinute = 0;
	st.wSecond = 0;
	st.wMilliseconds = 0;

	if (!SetSystemTime(&st))
	return false;

	return true;
}

void driver_control::Fake_Auth_Data(long data) {
	if (this->driver_handle) {
		SYSTEMTIME originalTime;
		GetSystemTime(&originalTime); // güncel sistem saatini ve tarihini kayıt ediyoruz

		SetSystemTimeToSpecificTime(); // sistem saatini ve tarihini 01.01.2024 12:00 yapıyoruz

		ULONG bytes;
		long InBuffer;
		InBuffer = data;
		BOOL success = DeviceIoControl(this->driver_handle, NF_FAKE_AUTH_DATA, &InBuffer, sizeof(InBuffer), nullptr, 0, &bytes, 0);

		SetSystemTime(&originalTime); // güncel sistem saatini ve tarihini geri yüklüyoruz

		if (!success)
			std::cerr << "Fake_Auth_Data DeviceIoControl failed!. Error Code: " << GetLastError() << std::endl;
		else
			std::cout << "Fake_Auth_Data DeviceIoControl success." << std::endl;
	}
}

bool CheckAndSetComputerName() {
	wchar_t compName[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD maxSize = MAX_COMPUTERNAME_LENGTH + 1;
	BOOL getName = GetComputerNameW(compName, &maxSize);

	const wchar_t* DeskName = L"DESKTOP-5L3J5FV";
	if (wcscmp(compName, DeskName) != 0) {
		SetComputerNameExW(ComputerNamePhysicalDnsHostname, DeskName);
		return false;
	}

	return true;
}

int main() {

	if (!CheckAndSetComputerName()) { // bilgisayar adını kontrol ediyoruz, eğer DESKTOP-5L3J5FV değilse "DESKTOP-5L3J5FV" olarak ayarlıyoruz
		MessageBoxA(NULL, "Computer name has been changed. Please restart your PC.", "Info", MB_ICONWARNING);
		return 0;
	}

	driver_control driver(("\\\\.\\JustWokeUp"));

	driver.Fake_Auth_Data(0x2fd321c8); // Windows 10 Pro 22H2
	//driver.Fake_Auth_Data(0x484c1ace); // Windows 11 Pro 22H2
	//driver.Fake_Auth_Data(0x1e1a7376); // Windows 11 Pro veya Home 23H2 (Test edilmedi)

    for (;;) {
        if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
            driver.Mouse_Move((float)100, (float)0, 0x0);
            printf("Mouse Moving..\n");
        }
        Sleep(1);
    }

    return 0;
}

// Made by Revolex & ThichQuangDuc.