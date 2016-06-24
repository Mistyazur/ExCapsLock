#include "keyboardhook.h"
#include "cmdpalette.h"
#include <QDebug>

CmdPalette *g_cmd = nullptr;

void KeyClick(WORD key)
{
    INPUT input[2] = {0};
    input[0].type = INPUT_KEYBOARD;
    input[0].ki.wVk = key;
    input[0].ki.dwFlags = 0;
    input[1].type = INPUT_KEYBOARD;
    input[1].ki.wVk = key;
//    input[1].ki.wVk = key+1;
    input[1].ki.dwFlags = KEYEVENTF_KEYUP;
    ::SendInput(_countof(input), input, sizeof(INPUT));
}

LRESULT CALLBACK KbHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    static bool capsDown = false;
    static bool bComposite  = false;
    static WPARAM prevWParam = 0;
    static DWORD prevVKeyCode = 0;
    PKBDLLHOOKSTRUCT pKey = (PKBDLLHOOKSTRUCT)lParam;

    // Key event injected
    if (pKey->flags & LLKHF_INJECTED)
        return CallNextHookEx(NULL, nCode, wParam, lParam);

    if (prevWParam != wParam || prevVKeyCode != pKey->vkCode)
    {
//        qDebug()<<"KEY:"<<wParam<<pKey->vkCode<<(pKey->flags & LLKHF_INJECTED);
        if (wParam == WM_KEYDOWN)
        {
            if (pKey->vkCode == VK_CAPITAL)
            {
                capsDown = true;
                bComposite  = false;
            }
            else
            {
                if (capsDown)
                {
                    bComposite  = true;

                    if (pKey->vkCode == VK_SPACE)
                    {
                        // Capslock + space: show command
                        g_cmd->show();
                    }
                    else if (pKey->vkCode == 'W')
                    {
                        // Capslock + W: Up
                        KeyClick(VK_UP);
                    }
                    else if (pKey->vkCode == 'S')
                    {
                        // Capslock + S: Down
                        KeyClick(VK_DOWN);
                    }
                    else if (pKey->vkCode == 'A')
                    {
                        // Capslock + A: Left
                        KeyClick(VK_LEFT);
                    }
                    else if (pKey->vkCode == 'D')
                    {
                        // Capslock + D: Right
                        KeyClick(VK_RIGHT);
                    }
                }
                else
                {
//                    if (pKey->vkCode == VK_ESCAPE)
//                    {
//                        // Esc: hide command
//                        g_cmd->hide();
//                    }
                }
            }
        }
        else if (wParam == WM_KEYUP)
        {
            if (pKey->vkCode == VK_CAPITAL)
            {
                capsDown = false;

                // If no key pressed after capslock then simulate ESC
                if (!bComposite)
                {
                    KeyClick(VK_ESCAPE);
                }
            }
        }

        prevWParam = wParam;
        prevVKeyCode = pKey->vkCode;
    }

    // Discard this message if it's my business
    if (pKey->vkCode == VK_CAPITAL || capsDown)
        return true;

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

KeyboardHook::KeyboardHook(QObject *parent) : QObject(parent)
{
    g_cmd = new CmdPalette();
    g_cmd->show();
    HHOOK keyboardHook = ::SetWindowsHookEx(WH_KEYBOARD_LL,
                                            KbHookProc,
                                            ::GetModuleHandle(NULL),
                                            0);
}

KeyboardHook::~KeyboardHook()
{
    delete g_cmd;
}
