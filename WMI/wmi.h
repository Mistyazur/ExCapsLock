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
        QAxObject *objIWbemLocator = new QAxObject("WbemScripting.SWbemLocator");
        QAxObject *objWMIService = objIWbemLocator->querySubObject("ConnectServer(QString&,QString&)",QString("."),QString("root\\cimv2"));
        QAxObject *objInterList = objWMIService->querySubObject("ExecQuery(QString&))", query);
        QAxObject *enum1 = objInterList->querySubObject("_NewEnum");
        IEnumVARIANT* enumInterface = 0;

        enum1->queryInterface((const QUuid)IID_IEnumVARIANT, (void**)&enumInterface);
        enumInterface->Reset();
        for (int i = 0; i < objInterList->dynamicCall("Count").toInt(); i++)
        {
            VARIANT *theItem = (VARIANT*)malloc(sizeof(VARIANT));
            if (enumInterface->Next(1,theItem,NULL) != S_FALSE)
            {
                QAxObject *item = new QAxObject((IUnknown *)theItem->punkVal);
                if(item)
                    vList += item->dynamicCall(property.toLatin1().data());
            }
        }
    }
};

#define WmiInstance Singleton<WMI>::instance()

#endif // WMI_H
