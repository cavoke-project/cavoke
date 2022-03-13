#ifndef CLIENT_PROTOTYPE_CACHE_MANAGER_H
#define CLIENT_PROTOTYPE_CACHE_MANAGER_H

#include <QDir>
#include <QStandardPaths>
#include <QString>
#include <QUrl>

namespace cache_manager {
const QDir CACHE_DIR =
    QDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation))
        .filePath("cavoke-client");
const QDir APPS_DIR = CACHE_DIR.filePath("apps");
const int BUFFER_SIZE = 1'000'000;

QUrl get_cached_app_path(const QString &gameId);

QUrl save_zip_to_cache(const QFile *archive,
                       const QString &gameId = "local-qml");

void unzip_to_folder(const QFile &archive, const QDir &dest);

}  // namespace cache_manager

#endif  // CLIENT_PROTOTYPE_CACHE_MANAGER_H
