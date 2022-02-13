#ifndef CLIENT_STARTCONTROLLER_H
#define CLIENT_STARTCONTROLLER_H

#include <QObject>

#include "startview.h"

class StartController : public QObject 
{
    Q_OBJECT
public:
    explicit StartController(QObject *parent = nullptr);

public slots:
    void showStartView();
    
private:
    StartView view;
    CavokeClientController *cavokeWindow;
};


#endif //CLIENT_STARTCONTROLLER_H
