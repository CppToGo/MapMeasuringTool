#ifndef WIN32HOOK_H
#define WIN32HOOK_H
#include <windows.h>
#include <aimwindow.h>
#include <qapplication.h>


LRESULT CALLBACK keyProc(int code, WPARAM wParam, LPARAM lParam); //钩子处理函数
HHOOK loadKeyHook();
void  unLoadHook(HHOOK& keyHook);

#endif // WIN32HOOK_H
