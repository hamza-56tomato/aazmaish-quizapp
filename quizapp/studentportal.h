#pragma once
#include <Qobject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "ui_quizapp.h"
class StudentPortal : public QObject
{
	Q_OBJECT
private:
	Ui::quizappClass* ui;
	QString idToken;
	/*QNetworkAccessManager* networkManager;
	QNetworkReply* networkReply;*/
public:
	StudentPortal(QObject* parent = nullptr, Ui::quizappClass * uiPtr = nullptr);
	~StudentPortal();
	void setIDToken(const QString& idToken_p);

public slots:
	
};