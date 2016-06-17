#include "keyboardhook.h"
#include "cmd.h"
#include <QDebug>

Cmd *g_cmd = nullptr;

void KeyClick(DWORD key)
{
    keybd_event(key, 0, 0, 0);
    keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
}

LRESULT CALLBACK KbHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    static bool capsDown = false;
    static bool keyDown = false;
    static WPARAM prevWParam = 0;
    static DWORD prevVKeyCode = 0;
    PKBDLLHOOKSTRUCT pKey = (PKBDLLHOOKSTRUCT)lParam;

    if (prevWParam != wParam || prevVKeyCode != pKey->vkCode)
    {
        qDebug()<<"KEY:"<<wParam<<pKey->vkCode;
        if (wParam == WM_KEYDOWN)
        {
            if (pKey->vkCode == VK_OEM_3)
            {
                capsDown = true;
                keyDown = false;
            }
            else
            {
                if (capsDown)
                {
                    keyDown = true;

                    if (pKey->vkCode == VK_SPACE)
                    {
                        // Capslock + space: show command
                        qDebug()<<"Show";
                        g_cmd->show();
                    }
                }
                else
                {
                    if (pKey->vkCode == VK_ESCAPE)
                    {
                        // Esc: hide command
                        g_cmd->hide();
                    }
                }
            }
        }
        else if (wParam == WM_KEYUP)
        {
            if (pKey->vkCode == VK_OEM_3)
            {
                capsDown = false;

                // If no key pressed after capslock then simulate ESC
                if (!keyDown)
                {
                    KeyClick(VK_ESCAPE);
                }
            }
        }

        prevWParam = wParam;
        prevVKeyCode = pKey->vkCode;
    }

    // Discard this message if it's my business
    if (pKey->vkCode == VK_OEM_3 || capsDown)
        return true;

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

KeyboardHook::KeyboardHook(QObject *parent) : QObject(parent)
{
    g_cmd = new Cmd();
    HHOOK keyboardHook = ::SetWindowsHookEx(WH_KEYBOARD_LL,
                                            KbHookProc,
                                            ::GetModuleHandle(NULL),
                                            0);
}

KeyboardHook::~KeyboardHook()
{
    delete g_cmd;
}
