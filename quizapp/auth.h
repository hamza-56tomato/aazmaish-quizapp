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
    
private:
	Ui::quizappClass* ui;
	QString apiKey;
	QString idToken;
	QNetworkAccessManager* networkManager;
	QNetworkReply* networkReply;
	void performPOST(const QString& url, const QJsonDocument& payload);
	bool is_signUp;
public slots:
	void on_networkReplyFinished();
signals:
	void userSignedIn(QString, bool);
};