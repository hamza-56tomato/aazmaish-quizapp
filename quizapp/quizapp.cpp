#include "quizapp.h"
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <qDebug>
quizapp::quizapp(QWidget* parent)
    : QMainWindow(parent)
{
	teacherPortal = nullptr;
	studentPortal = nullptr;
    ui.setupUi(this);
	ui.stackedWidget->setCurrentWidget(ui.welcomePage);
    this->showMaximized();
	isTeacher = false;
	networkManager = new QNetworkAccessManager(this);
	networkReply = nullptr;
	auth = new Auth(this, &ui);
	auth->setAPIkey("AIzaSyDi8hYQRv509aVPL_H7lkMB3_7lUqEAzAQ");
	connect(ui.teacher_code_btn, &QPushButton::clicked, this, &quizapp::teacherCodeVerification);
	connect(ui.teacher_code_input, &QLineEdit::returnPressed, this, &quizapp::teacherCodeVerification);
	connect(auth, &Auth::userSignedIn, this, &quizapp::on_userSignedIn);
}

quizapp::~quizapp()
{
	delete auth;
	networkManager->deleteLater();
}

void quizapp::on_userSignedIn(QString idToken_p, bool is_signUp)
{
	QString email;
	QString name;
	idToken = idToken_p;
	if (isTeacher) {
		ui.stackedWidget->setCurrentWidget(ui.teacherPortal);
		teacherPortal = new TeacherPortal(this, &ui);
		teacherPortal->setIDToken(idToken);
		if (is_signUp) {
			teacherPortal->setTeacherName(ui.signup_name_input->text());
			teacherPortal->setTeacherEmail(ui.signup_email_input->text());
			sendUserData();
		}
		else {
			getUsers();
		}
	}
	else {
		ui.stackedWidget->setCurrentWidget(ui.studentPortal);
		studentPortal = new StudentPortal(this, &ui);
		studentPortal->setIDToken(idToken);
		if (is_signUp) {
			sendUserData();
			studentPortal->setStudentName(ui.signup_name_input->text());
			studentPortal->setStudentEmail(ui.signup_email_input->text());
		}
		else {
			getUsers();
		}
	}
}
void quizapp::getUsers() {
	networkReply = networkManager->get(QNetworkRequest(QUrl("https://aazmaish-quizapp-default-rtdb.asia-southeast1.firebasedatabase.app/users.json")));
	connect(networkReply, &QNetworkReply::finished, this, [this]() {
		QByteArray response = networkReply->readAll();
		QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
		QJsonObject users = jsonDoc.object();

		if (users.contains("error")) {
			QJsonObject error = users["error"].toObject();
			QMessageBox::warning(nullptr, "Error", error["message"].toString());
			return;
		}
		for (QString key : users.keys()) {
			QJsonObject userObject = users.value(key).toObject();
			QString email = ui.login_email_input->text();
			if (userObject["email"].toString() == email) {
				if (isTeacher) {
					teacherPortal->setTeacherName(userObject["name"].toString());
					teacherPortal->setTeacherEmail(userObject["email"].toString());
				}
				else {
					studentPortal->setStudentName(userObject["name"].toString());
					studentPortal->setStudentEmail(userObject["email"].toString());
				}
				break;
			}
		}
		networkReply->deleteLater();
		ui.login_email_input->clear();
		ui.login_password_input->clear();
	});
}

void quizapp::on_login_btn_clicked()
{
	QString email = ui.login_email_input->text();
	QString password = ui.login_password_input->text();
	auth->signIn(email, password);
	
}
		

void quizapp::on_goto_signup_btn_clicked()
{
	ui.stackedWidget->setCurrentWidget(ui.signUpPage);
}
void quizapp::on_student_btn_clicked()
{
	isTeacher = false;
	ui.stackedWidget->setCurrentWidget(ui.loginPage);
}
void quizapp::on_teacher_btn_clicked()
{
	ui.stackedWidget->setCurrentWidget(ui.teacherCodePage);
}
void quizapp::teacherCodeVerification()
{
	QString code = ui.teacher_code_input->text();
	ui.teacher_code_input->clear();
	if (code == "1234") {
		isTeacher = true;
		ui.stackedWidget->setCurrentWidget(ui.loginPage);
	}
	else {
		QMessageBox::warning(nullptr, "Error", "Invalid code");
		ui.stackedWidget->setCurrentWidget(ui.welcomePage);
	}
}
void quizapp::on_login_back_btn_clicked()
{
	ui.stackedWidget->setCurrentWidget(ui.welcomePage);
}
void quizapp::on_signup_back_btn_clicked()
{
	ui.stackedWidget->setCurrentWidget(ui.loginPage);
}
void quizapp::on_signup_btn_clicked()
{
	QString email = ui.signup_email_input->text();
	QString password = ui.signup_password_input->text();
	auth->signUp(email, password);
	
}
void quizapp::sendUserData() {
	QNetworkRequest request(QUrl("https://aazmaish-quizapp-default-rtdb.asia-southeast1.firebasedatabase.app/users.json"));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	QJsonObject userObject;
	userObject["name"] = ui.signup_name_input->text();
	userObject["email"] = ui.signup_email_input->text();
	QJsonDocument userDoc(userObject);
	networkReply = networkManager->post(request, userDoc.toJson());
	connect(networkReply, &QNetworkReply::finished, this, &quizapp::on_userData_sent);
	ui.signup_email_input->clear();
	ui.signup_password_input->clear();
	ui.signup_name_input->clear();
}
void quizapp::on_userData_sent() {
	QByteArray response = networkReply->readAll();
	QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
	if (jsonDoc.object().contains("error")) {
		QJsonObject error = jsonDoc.object()["error"].toObject();
		QMessageBox::warning(nullptr, "Error", error["message"].toString());
		return;
	}
	QMessageBox::information(nullptr, "Success", "You have been signed in Successfully");
	networkReply->deleteLater();
}