#pragma once

#include <QObject>
#include "ui_quizapp.h"
#include <QList>
#include <QButtonGroup>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
class TeacherPortal : public QObject
{
    Q_OBJECT

public:
    TeacherPortal(QObject* parent = nullptr, Ui::quizappClass* uiPtr = nullptr);
    ~TeacherPortal();
    void setIDToken(const QString& idToken_p);
    void setTeacherName(const QString& name);
	void setTeacherEmail(const QString& email);
private:
    Ui::quizappClass* ui;
    QString idToken;
    int questionCount;
	QNetworkAccessManager* networkManager;
	QNetworkReply* networkReply;
    QString teacherName;
	QString teacherEmail;

    struct QuestionEntry {
        QLineEdit* question;
        QLineEdit* optionA;
        QLineEdit* optionB;
        QLineEdit* optionC;
        QLineEdit* optionD;
        QButtonGroup* correctAnswerGroup;
    };
    struct QuizzesData {
		QString quizName;
		QString quizTime;
        QString quizID;
    };
    QList<QuestionEntry> questionsList;
	QList<QuizzesData> quizzesList;
public slots:
    void addNewQuestion();
    void on_create_quiz_btn_clicked();
    void on_submit_quiz_button_clicked();
    void on_create_quiz_reply_finished();
    void on_signOut_btn_clicked();
    void on_create_quiz_back_btn_clicked();
    void on_edit_quiz_back_btn_clicked();
    void clear_quiz_inputs();

	void on_edit_quiz_btn_clicked();
	void generate_quizzes_to_edit();
};
