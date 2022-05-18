#include "AuthenticationManager.h"
#include <QDesktopServices>
#include <QOAuthHttpServerReplyHandler>
#include <QtDebug>

//#if defined(INCLUDE_OWN_QT_KEYCHAIN)
//#include "keychain.h"
//#else
//#include <qt5keychain/keychain.h>
//#endif

cavoke::auth::AuthenticationManager::AuthenticationManager() {
    // setup oauth settings
    oauth2.setAuthorizationUrl(QUrl(authorizationUrl));
    oauth2.setAccessTokenUrl(QUrl(accessTokenUrl));
    oauth2.setClientIdentifier(clientId);
    oauth2.setScope(scope);

    connect(&oauth2, &QOAuth2AuthorizationCodeFlow::statusChanged,
            [=](QAbstractOAuth::Status status) {
                if (status == QAbstractOAuth::Status::Granted) {
                    qDebug() << "Authenticated!!";
                    writeSecurePassword(oauth2.refreshToken());
                    emit authenticated();
                } else if (status == QAbstractOAuth::Status::NotAuthenticated) {
                    qWarning() << "Failed authentication";
                }
            });
    oauth2.setModifyParametersFunction(
        [&](QAbstractOAuth::Stage stage, auto *parameters) {
            if (stage == QAbstractOAuth::Stage::RequestingAuthorization)
                parameters->insert("audience", audience);
        });
    connect(&oauth2, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser,
            &QDesktopServices::openUrl);

    readSecurePassword([&](const QString &refresh_token) {
        qDebug() << "Loaded refresh token from Keychain!";
        oauth2.setRefreshToken(refresh_token);
        oauth2.refreshAccessToken();
    });
}
bool cavoke::auth::AuthenticationManager::checkAuthStatus() {
    return oauth2.status() !=
           QOAuth2AuthorizationCodeFlow::Status::NotAuthenticated;
}
void cavoke::auth::AuthenticationManager::writeSecurePassword(
    const QString &pass) {
    // FIXME: move to qkeychain
    settings.setValue(profile, pass);
    //    auto *job = new QKeychain::WritePasswordJob("cavoke_keychain");
    //    job->setAutoDelete(false);
    //    job->setInsecureFallback(false);
    //    job->setKey(profile);
    //    job->setTextData(pass);
    //    connect(job, &QKeychain::WritePasswordJob::finished, this,
    //            [](QKeychain::Job *job) {
    //                if (job->error()) {
    //                    qWarning() << job->errorString();
    //                }
    //                job->deleteLater();
    //            });
    //    job->start();
}
template <typename L>
void cavoke::auth::AuthenticationManager::readSecurePassword(L callback) {
    // FIXME: move to qkeychain
    callback(settings.value(profile).toString());
    //    auto *job = new QKeychain::ReadPasswordJob("cavoke_keychain");
    //    job->setAutoDelete(false);
    //    job->setInsecureFallback(false);
    //    job->setKey(profile);
    //    connect(job, &QKeychain::ReadPasswordJob::finished, this,
    //            [=](QKeychain::Job *job) {
    //                if (job->error()) {
    //                    qWarning() << job->errorString();
    //                }
    //                auto readJob = dynamic_cast<QKeychain::ReadPasswordJob
    //                *>(job); callback(readJob->textData());
    //                job->deleteLater();
    //            });
    //    job->start();
}
void cavoke::auth::AuthenticationManager::deleteSecurePassword() {
    // FIXME: move to qkeychain
    settings.setValue(profile, "");
    //    auto *job = new QKeychain::DeletePasswordJob("cavoke_keychain");
    //    job->setAutoDelete(false);
    //    job->setInsecureFallback(false);
    //    job->setKey(profile);
    //    job->setProperty("profile", profile);
    //    connect(job, &QKeychain::WritePasswordJob::finished, this,
    //            [](QKeychain::Job *job) {
    //                if (job->error()) {
    //                    qWarning() << job->errorString();
    //                }
    //                job->deleteLater();
    //            });
    //    job->start();
}

void cavoke::auth::AuthenticationManager::logout() {
    deleteSecurePassword();
    oauth2.setRefreshToken("");
    // Immediately asks user to relogin
    // Terrible solution, couldn't find anything better
    // At least he can decline, so fine....
    oauth2.grant();
}

const QString cavoke::auth::AuthenticationManager::profile = "cavoke_user";
const QString cavoke::auth::AuthenticationManager::authorizationUrl =
    "https://cavoke.eu.auth0.com/authorize";
const QString cavoke::auth::AuthenticationManager::accessTokenUrl =
    "https://cavoke.eu.auth0.com/oauth/token";
const QString cavoke::auth::AuthenticationManager::clientId =
    "yxkEiSikGF6JSaFwIikeLQlUNAUUR0ak";
const QString cavoke::auth::AuthenticationManager::scope =
    "identity sessions profile users offline_access";
const QString cavoke::auth::AuthenticationManager::audience =
    "https://develop.api.cavoke.wlko.me";  // NOTE: should not equal to HOST, as
                                           // this must be registered as API
                                           // endpoint. Basically don't change
                                           // this.
