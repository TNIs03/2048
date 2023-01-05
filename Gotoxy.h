#pragma once
#include <Windows.h>
CONSOLE_SCREEN_BUFFER_INFO csbi;

void gotoxy(int x, int y) {
    static HANDLE h = NULL;
    if (!h)
        h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD c = { x, y };
    SetConsoleCursorPosition(h, c);
}

void gotoxy(COORD c) {
    static HANDLE h = NULL;
    if (!h)
        h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(h, c);
}

COORD GetCursorPos() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD c = { 0, 0 };
    if (GetConsoleScreenBufferInfo(h, &csbi)) c = csbi.dwCursorPosition;
    return c;
}