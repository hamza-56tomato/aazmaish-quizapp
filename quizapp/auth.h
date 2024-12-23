#pragma once
#include <Qobject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <ui_quizapp.h>
class Auth : public QObject
{
	Q_OBJECT
public:
	Auth(QObject* parent = nullptr, Ui::quizappClass *uiPtr = nullptr);
	~Auth();
	void setAPIkey(const QString& key);
	void signUp(const QString& email, const QString& password);
	void signIn(const QString& email, const QString& password);
    
	void signOut();
private:
	Ui::quizappClass* ui;
	QString apiKey;
	QString idToken;
	QNetworkAccessManager* networkManager;
	QNetworkReply* networkReply;
	void performPOST(const QString& url, const QJsonDocument& payload);
	void parseReponse(const QByteArray& response);
public slots:
	void on_networkReplyFinished();
signals:
	void userSignedIn();
};