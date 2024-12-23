#include "auth.h"
#include <QJsonObject>
#include <QNetworkRequest>
#include <QMessageBox>
Auth::Auth(QObject* parent, Ui::quizappClass *uiPtr): QObject(parent), apiKey(QString()), ui(uiPtr)
{
	networkManager = new QNetworkAccessManager(this);
	networkReply = nullptr;
}
Auth::~Auth()
{
	networkManager->deleteLater();
}
void Auth::setAPIkey(const QString& key)
{
	apiKey = key;
}
void Auth::signUp(const QString& email, const QString& password)
{
	QJsonDocument payload;
	QVariantMap payloadMap;
	payloadMap["email"] = email;
	payloadMap["password"] = password;
	payloadMap["returnSecureToken"] = true;
	payload.setObject(QJsonObject::fromVariantMap(payloadMap));
	performPOST("https://identitytoolkit.googleapis.com/v1/accounts:signUp?key=" + apiKey, payload);
}

void Auth::signIn(const QString& email, const QString& password)
{
	QJsonDocument payload;
	QVariantMap payloadMap;
	payloadMap["email"] = email;
	payloadMap["password"] = password;
	payloadMap["returnSecureToken"] = true;
	payload.setObject(QJsonObject::fromVariantMap(payloadMap));
	performPOST("https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword?key=" + apiKey, payload);
}

void Auth::performPOST(const QString& url, const QJsonDocument& payload)
{
	QNetworkRequest networkRequest((QUrl(url)));
	networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	networkReply = networkManager->post(networkRequest, payload.toJson());
	connect(networkReply, &QNetworkReply::finished, this, &Auth::on_networkReplyFinished);
}
void Auth::on_networkReplyFinished() {
	QByteArray response = networkReply->readAll();
	ui->status->setText(response);
	ui->stackedWidget->setCurrentWidget(ui->signupSuccess);
	parseReponse(response);
	networkReply->deleteLater();
}
void Auth::parseReponse(const QByteArray& response) {
	QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
	if (jsonDoc.object().contains("error")) {
		QJsonObject error = jsonDoc.object()["error"].toObject();
		QMessageBox::warning(nullptr, "Error", error["message"].toString() + "\nPlease return to login page");
		ui->stackedWidget->setCurrentWidget(ui->loginPage);
		return;
	}
	if (jsonDoc.object().contains("kind")) {
		idToken = jsonDoc.object()["idToken"].toString();
		emit userSignedIn();
	}

	
}
