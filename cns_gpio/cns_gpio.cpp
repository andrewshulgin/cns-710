#include "stdafx.h"
#include <windows.h>
#include <commctrl.h>
#include <cstdlib>

#define MODE_IN 0
#define MODE_OUT 1

#define STATE_HIGH 1
#define STATE_LOW 0

struct gpio_cmd{};

struct set_pin_mode_cmd : public gpio_cmd {
	int pin;
	int mode;
	int v2;
	int state;
};

struct set_pin_state_cmd : public gpio_cmd {
	int pin;
	int state;
};

void send_command(int code, gpio_cmd &cmd)
{
	HANDLE handle = CreateFileW(L"SYS1:", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (handle == (HANDLE)-1)
		fprintf(stderr, "ERROR: SYS1 OPEN\r\n");
    DeviceIoControl(handle, code, &cmd, sizeof(&cmd), NULL, NULL, NULL, NULL);
	CloseHandle(handle);
}

void set_mode(int pin, int mode, int state)
{
    struct set_pin_mode_cmd cmd;
    cmd.pin = pin;
    cmd.mode = mode;
    cmd.v2 = mode + 1;
    cmd.state = state;
    send_command(0x10468C0, cmd);
}

void set_state(int pin, int state)
{
    struct set_pin_state_cmd cmd;
    cmd.pin = pin;
    cmd.state = state;
    send_command(0x10468C8, cmd);
}

int _tmain(int argc, _TCHAR* argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <PIN 0|1> [MODE 0|1]\r\n", argv[0]);
        return 1;
    }
    set_mode(_wtoi(argv[1]), MODE_OUT, argc > 2 ? _wtoi(argv[2]) : STATE_LOW);
    while(argc == 2 && true){
        set_state(_wtoi(argv[1]), STATE_HIGH);
        Sleep(500);
        set_state(_wtoi(argv[1]), STATE_LOW);
        Sleep(500);
    }
    return 0;
}
