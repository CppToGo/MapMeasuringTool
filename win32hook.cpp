#include "win32hook.h"
#include <qdebug.h>

HHOOK keyHook = nullptr ;

LRESULT CALLBACK keyProc(int nCode, WPARAM wParam, LPARAM lParam){
    KBDLLHOOKSTRUCT *e = (KBDLLHOOKSTRUCT *)lParam ;
    if (HC_ACTION == nCode && GetAsyncKeyState((int)e->vkCode) & 0x8000){
        switch (e->vkCode) {
        case VK_UP:
            AimWindow::getInstance()->setAlphaValue();
            break;
        case VK_DOWN:
           if(!AimWindow::getInstance()->isMinimized()){
               AimWindow::getInstance()->showMinimized();
           }else{
               AimWindow::getInstance()->showMaximized();
           }
            break;
        case VK_LEFT:
            AimWindow::delInstance();
            qApp->quit();
            break;
        }
    }
    return CallNextHookEx(keyHook , nCode , wParam , lParam);
}

HHOOK loadKeyHook(){
    return SetWindowsHookExW(WH_KEYBOARD_LL , keyProc , GetModuleHandleA(nullptr), 0);
}

void unLoadHook(HHOOK& hook){
    UnhookWindowsHookEx(hook);
    hook = nullptr ;
    qDebug()<< "unLoadHook keyHook Status : "<<keyHook ;
}
