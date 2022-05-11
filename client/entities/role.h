#ifndef CAVOKE_ROLE_H
#define CAVOKE_ROLE_H

#include <QString>
struct Role {
    QString name = "";
    int id = 0;

    Role(QString _name, int _id);
};

#endif  // CAVOKE_ROLE_H
