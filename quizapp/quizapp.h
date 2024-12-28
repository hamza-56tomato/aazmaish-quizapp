#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_quizapp.h"
#include "studentportal.h"
#include "teacherportal.h"
#include "auth.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
class quizapp : public QMainWindow
{
	Q_OBJECT

public:
	quizapp(QWidget *parent = nullptr);
	~quizapp();
	QNetworkAccessManager* networkManager;
	QNetworkReply* networkReply;

private:
	Ui::quizappClass ui;
	StudentPortal* studentPortal;
	TeacherPortal* teacherPortal;
	Auth* auth;
	bool isTeacher;
	QString idToken;
	bool signUp;
	void getUsers();

public slots: 
	void on_student_btn_clicked();
	void on_teacher_btn_clicked();
	void on_login_btn_clicked();
	void on_goto_signup_btn_clicked();
	void on_login_back_btn_clicked();
	void on_signup_back_btn_clicked();
	void on_signup_btn_clicked();
	void on_userSignedIn(QString idToken_p, bool is_signUp);
	void teacherCodeVerification();
	void sendUserData();
	void on_userData_sent();
};
