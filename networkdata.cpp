#include "networkdata.h"

#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QEventLoop>

//delete later
#include <QDebug>

// using API from website ipdata.co
// max for per-dey 1500 requests
// for future look into another website
NetworkData::NetworkData(QObject *parent) : QObject(parent)
{
    // another json request but more secure
    // https://api.ipdata.co/?api-key=197a17ae219105fc92c515c579215ef9527cf80897c7a751d106625e
    // but it doesn`t understand most of cities
    network_manager = new QNetworkAccessManager;
    network_manager->get(QNetworkRequest(QUrl("http://ip-api.com/json/")));
    QEventLoop event;
    connect(network_manager, &QNetworkAccessManager::finished, this, &NetworkData::replyUserData);
    // we need this because we must to wait to retrieve data from Network
    connect(network_manager,&QNetworkAccessManager::finished, &event, &QEventLoop::quit);
    event.exec();
}

//Methods @set
void NetworkData::setUserIp(QString data)
{
   user_ip = data;
}

void NetworkData::setUserCity(QString data)
{
   user_city = data;
}

void NetworkData::setUserCountry(QString data)
{
   user_country = data;
}

//Methods @get
QString NetworkData::getUserIp()
{
   return user_ip;
}

QString NetworkData::getUserCity()
{
   return user_city;
}

QString NetworkData::getUserCountry()
{
   return user_country;
}

//function that helps to get: ip,city,country
// from website that gives in JSON file data
void NetworkData::replyUserData(QNetworkReply *reply)
{
    if (!reply->error())
    {
        // read all from request all data that we have at that website
         QJsonDocument jsondocument = QJsonDocument::fromJson(reply->readAll());
         QJsonObject root = jsondocument.object();

       //  QJsonValue ip_value = root["ip"];
         QJsonValue ip_value = root["query"];
         setUserIp(ip_value.toString());

         QJsonValue city_value = root["city"];
         setUserCity(city_value.toString());

         //QJsonValue country_value = root["country_name"];
         QJsonValue country_value = root["country"];
         setUserCountry(country_value.toString());
    }
    else
        qDebug()<<"error-> "<<reply->errorString();
    reply->deleteLater();
}
