#ifndef CLIENT_PROTOTYPE_CACHE_MANAGER_H
#define CLIENT_PROTOTYPE_CACHE_MANAGER_H

#include <QString>
#include <QStandardPaths>
#include <QDir>

namespace cache_manager {
    const QDir CACHE_DIR = QDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)).filePath(
            "cavoke-client");
    const QDir APPS_DIR = CACHE_DIR.filePath("apps");
    const int BUFFER_SIZE = 1'000'000;

    QString get_cached_app_path(QString app_name);

    QString save_zip_to_cache(QFile archive);

    void unzip_to_folder(QFile &archive, const QDir &dest);


}

#endif //CLIENT_PROTOTYPE_CACHE_MANAGER_H
