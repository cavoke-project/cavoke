#ifndef CAVOKE_CLIENT_NETWORK_MANAGER_H
#define CAVOKE_CLIENT_NETWORK_MANAGER_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
struct NetworkManager : public QObject {
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = nullptr);
    
public slots:
    void doTestHealthCheck();
    void getGamesList();
    
private slots:
    void doneTestHealthCheck(QNetworkReply *reply);  
    void gotGamesList(QNetworkReply *reply);

private:
    QNetworkAccessManager manager;
    const static inline QUrl HOST{"http://0.0.0.0:12345"};  // FIXME: in conf
};

#endif  // CAVOKE_CLIENT_NETWORK_MANAGER_H
