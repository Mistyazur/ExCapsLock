#ifndef WMI_H
#define WMI_H

#include <QAxObject>
#include <QUuid>
#include <Windows.h>
#include <singleton.h>

class WMI
{
public:
    WMI()
    {
        CoInitializeEx(0, COINIT_MULTITHREADED);
    }

    void get(const QString &query, const QString &property, QVariantList &vList)
    {
        static VARIANT *theItem = new VARIANT;
        static QAxObject *objIWbemLocator = new QAxObject("WbemScripting.SWbemLocator");
        static QAxObject *objWMIService = objIWbemLocator->querySubObject("ConnectServer(QString&,QString&)",QString("."),QString("root\\cimv2"));
        QAxObject *objInterList = objWMIService->querySubObject("ExecQuery(QString&))", query);
        QAxObject *enum1 = objInterList->querySubObject("_NewEnum");
        IEnumVARIANT* enumInterface = 0;

        enum1->queryInterface((const QUuid)IID_IEnumVARIANT, (void**)&enumInterface);
        enumInterface->Reset();
        int resCount = objInterList->dynamicCall("Count").toInt();
        for (int i = 0; i < resCount; ++i)
        {
            if (enumInterface->Next(1, theItem, NULL) != S_FALSE)
            {
                QAxObject item((IUnknown *)theItem->punkVal);
                vList += item.dynamicCall(property.toLatin1().data());
            }
        }
    }
};

#define WmiInstance Singleton<WMI>::instance()

#endif // WMI_H
