#pragma once
#include <Qobject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "ui_quizapp.h"

class StudentPortal : public QObject
{
	Q_OBJECT
public:
	StudentPortal(Ui::quizappClass* uiPtr, QObject* parent = nullptr);
	~StudentPortal();

private:
	Ui::quizappClass *ui;
	QNetworkAccessManager* networkManager;
	QNetworkReply* networkReply;

private slots:
	void on_login_btn_clicked();
	void on_back_btn_clicked();
	void on_back_btn_2_clicked();
	void handleAnswer();
	void handleInput();
	void on_submit_btn_clicked();
	void on_networkReplyFinished();
};