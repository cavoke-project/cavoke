#include "AuthenticationManager.h"
#include <QDesktopServices>
#include <QOAuthHttpServerReplyHandler>

cavoke::auth::AuthenticationManager::AuthenticationManager() {
    // setup oauth settings
    oauth2.setAuthorizationUrl(QUrl(
        "https://cavoke.eu.auth0.com/authorize"));  // FIXME: move to constants
    oauth2.setAccessTokenUrl(
        QUrl("https://cavoke.eu.auth0.com/oauth/token"));  // FIXME: move to
                                                           // constants
    oauth2.setClientIdentifier(
        "yxkEiSikGF6JSaFwIikeLQlUNAUUR0ak");  // FIXME: move to constants
    oauth2.setScope(
        "identity sessions profile users");  // FIXME: move to constants

    connect(&oauth2, &QOAuth2AuthorizationCodeFlow::statusChanged,
            [=](QAbstractOAuth::Status status) {
                if (status == QAbstractOAuth::Status::Granted) {
                    qDebug() << "Authenticated!!";
                    emit authenticated();
                }
                // TODO: fatal error if unauthenticated
            });
    oauth2.setModifyParametersFunction(
        [&](QAbstractOAuth::Stage stage, auto *parameters) {
            if (stage == QAbstractOAuth::Stage::RequestingAuthorization)
                parameters->insert("audience",
                                   "https://develop.api.cavoke.wlko.me");
            // TODO: use HOST
        });
    connect(&oauth2, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser,
            &QDesktopServices::openUrl);
}
bool cavoke::auth::AuthenticationManager::checkAuthStatus() {
    // TODO: credentials recovery
    return oauth2.status() !=
           QOAuth2AuthorizationCodeFlow::Status::NotAuthenticated;
}
