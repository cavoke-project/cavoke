#ifndef CAVOKE_VALIDATIONRESULT_H
#define CAVOKE_VALIDATIONRESULT_H

#include <QtCore/QJsonObject>
#include <QtCore/QString>
struct ValidationResult {
public:
    ValidationResult();
    ValidationResult(bool _success, QString _message);

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    bool success = false;
    QString message = "Empty result";

private:
    static inline const QString SUCCESS = "success";
    static inline const QString MESSAGE = "message";
};

#endif  // CAVOKE_VALIDATIONRESULT_H
