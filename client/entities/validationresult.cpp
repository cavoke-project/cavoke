#include "validationresult.h"
#include <utility>
ValidationResult::ValidationResult() = default;
ValidationResult::ValidationResult(bool _success, QString _message)
    : success(_success), message(std::move(_message)) {
}

void ValidationResult::read(const QJsonObject &json) {
    if (json.contains(SUCCESS) && json[SUCCESS].isBool()) {
        success = json[SUCCESS].toBool();
    }
    if (success) {
        message = "Now you can start the game!";
    }
    if (!success && json.contains(MESSAGE) && json[MESSAGE].isString()) {
        message = json[MESSAGE].toString();
    }
}

void ValidationResult::write(QJsonObject &json) const {
    json[SUCCESS] = success;
    if (!success) {
        json[MESSAGE] = message;
    }
}
