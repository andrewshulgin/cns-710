#include "stdafx.h"
#include <windows.h>
#include <commctrl.h>
#include <cstdlib>

#define STATE_OFF 0
#define STATE_ON 1

struct backlight_cmd{};

struct set_state_cmd : public backlight_cmd {
	int state;
};

struct set_level_cmd : public backlight_cmd {
	int level;
};

void send_command(int code, backlight_cmd &cmd)
{
	HANDLE handle = CreateFileW(L"BKL1:", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (handle == (HANDLE)-1)
		fprintf(stderr, "ERROR: BKL1 OPEN\r\n");
    DeviceIoControl(handle, code, &cmd, sizeof(&cmd), NULL, NULL, NULL, NULL);
	CloseHandle(handle);
}

void set_state(int state)
{
    struct set_state_cmd cmd;
    cmd.state = state;
    send_command(0x1046804, cmd);
}

void set_level(int level)
{
    struct set_level_cmd cmd;
    cmd.level = level;
    send_command(0x1046800, cmd);
}

int _tmain(int argc, _TCHAR* argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <STATE ON|OFF> [LEVEL 0..100]\r\n", argv[0]);
        return 1;
    }
    if (wcscmp(argv[1], L"ON") == 0)
    {
        set_state(STATE_ON);
    }
    if (wcscmp(argv[1], L"OFF") == 0)
    {
        set_state(STATE_OFF);
    }
    if (argc > 2) {
        set_level(_wtoi(argv[2]));
    }
    return 0;
}
