#ifndef CAVOKE_AUTHENTICATIONMANAGER_H
#define CAVOKE_AUTHENTICATIONMANAGER_H

#include <QOAuth2AuthorizationCodeFlow>

namespace cavoke::auth {
/// Manages all Oauth2 related things. Including code flow itself and storing
/// refresh tokens.
struct AuthenticationManager : public QObject {
    Q_OBJECT
public:
    AuthenticationManager();
    QOAuth2AuthorizationCodeFlow oauth2;
    /// Singleton wrapper
    static AuthenticationManager &getInstance() {
        static AuthenticationManager obj;
        return obj;
    }
    bool checkAuthStatus();
public slots:
    void requestNameChange(const QString &new_name);
    void fetchName();
signals:
    void authenticated();
    QString nameUpdated();
private:
    QString name;
};
}  // namespace cavoke::auth

#endif  // CAVOKE_AUTHENTICATIONMANAGER_H
