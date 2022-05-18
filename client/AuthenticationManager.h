#ifndef CAVOKE_AUTHENTICATIONMANAGER_H
#define CAVOKE_AUTHENTICATIONMANAGER_H

#include <QOAuth2AuthorizationCodeFlow>
#include <QSettings>

namespace cavoke::auth {
/// Manages all Oauth2 related things. Including code flow itself and storing
/// refresh tokens.
struct AuthenticationManager : public QObject {
    Q_OBJECT
public:
    AuthenticationManager() = default;
    QOAuth2AuthorizationCodeFlow oauth2;
    /// Singleton wrapper
    static AuthenticationManager &getInstance() {
        static AuthenticationManager obj;
        return obj;
    }
    bool checkAuthStatus();
    void init();
    void relogin();
signals:
    void authenticated();

private:
    void writeSecurePassword(const QString &profile, const QString &pass);
    template <typename L>
    void readSecurePassword(const QString &profile, L callback);
    void deleteSecurePassword(const QString &profile);

    QSettings settings{this};  // FIXME: move to qtkeychain

    const static QString authorizationUrl;
    const static QString accessTokenUrl;
    const static QString clientId;
    const static QString scope;
    const static QString audience;

    const static QString refresh_token_profile;
};
}  // namespace cavoke::auth

#endif  // CAVOKE_AUTHENTICATIONMANAGER_H
