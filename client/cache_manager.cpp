#include <QtDebug>

#include <cache_manager.h>
#include <kzip.h>

QString cache_manager::get_cached_app_path(QString app_name) {
    QDir cur_app_dir = QDir(APPS_DIR.filePath(app_name));
    if (!cur_app_dir.exists()) {
        return QString();
    }
    QFile qml_file(cur_app_dir.filePath("app.qml"));
    if (!qml_file.exists()) {
        qDebug() << "Can not find app.qml in directory " << cur_app_dir << '\n';
        return QString();
    }
    return qml_file.fileName();
}

QString cache_manager::save_zip_to_cache(const QFile *archive_file) {
    QFileInfo archiveFileInfo(archive_file->fileName());
    QString app_name = archiveFileInfo.baseName();
    QDir app_dir(APPS_DIR.filePath(app_name));

    if (app_dir.exists()) {
        app_dir.removeRecursively();
    }

    unzip_to_folder(*archive_file, APPS_DIR);

    return app_dir.filePath("app.qml");
}

void cache_manager::unzip_to_folder(const QFile &archive_file, const QDir &dest_dir) {
    if (!archive_file.exists()) {
        qDebug() << "Can not unpack the archive: " << archive_file.fileName() << " does not exists\n";
        return;
    }
    dest_dir.mkpath(".");

    KZip archive(archive_file.fileName());
    if (!archive.open(QIODevice::ReadOnly)) {
        qDebug() << "Cannot open " << archive_file.fileName() << " Is it a valid zip file?\n";
        return;
    }

    const KArchiveDirectory *archive_root = archive.directory();
    archive_root->copyTo(dest_dir.path(), true);
    archive.close();
    qDebug() << "Unpacked " << archive_file.fileName() << " to " << dest_dir.path() << '\n';
    archive.close();
}
