#ifndef NETWORKDATA_H
#define NETWORKDATA_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QByteArray>
#include <QJsonObject>
#include <QTimer>

/*
     this class provides data
     from user login to app
     with this data
     user can manage with his session
*/

class NetworkData : public QObject
{
    Q_OBJECT
public:
    explicit NetworkData(QObject *parent = nullptr);

 //Methods @set
    void setUserIp(QString data);
    void setUserCity(QString data);
    void setUserCountry(QString data);

//Methods @get
    QString getUserIp();
    QString getUserCity();
    QString getUserCountry();

public slots:
    //function that helps to get: ip,city,country
    // from website that gives in JSON file data
    void replyUserData(QNetworkReply *reply);
private:
    QString user_ip, user_city, user_country;
    QNetworkAccessManager *network_manager;
};
#endif
