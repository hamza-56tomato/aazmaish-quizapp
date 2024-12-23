#include <studentportal.h>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>

StudentPortal::StudentPortal(Ui::quizappClass* uiPtr, QObject* parent)
	: QObject(parent), ui(uiPtr)
{
	/*networkManager = new QNetworkAccessManager(this);
	networkReply = networkManager->get(QNetworkRequest(QUrl("https://aazmaish-quizapp-default-rtdb.asia-southeast1.firebasedatabase.app/Quizzes.json")));
	connect(networkReply, &QNetworkReply::finished, this, &StudentPortal::on_networkReplyFinished);*/
	
}
StudentPortal::~StudentPortal()
{
	
}

