#ifndef STUDENTPORTAL_H
#define STUDENTPORTAL_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QButtonGroup>
#include <QRadioButton>
#include <ui_quizapp.h>


class StudentPortal : public QObject {
    Q_OBJECT

public:
    explicit StudentPortal(QObject* parent = nullptr, Ui::quizappClass* uiPtr = nullptr);
    ~StudentPortal();

	void setIDToken(const QString& idToken_p);
	void setStudentName(const QString& name);
	void setStudentEmail(const QString& email);
private:
    Ui::quizappClass* ui;
    QNetworkAccessManager* manager;
	QString idToken;
	int marks; // to store the marks of the student on current quiz attempt
    struct Quiz {
        QString quizName;
        QString teacherName;
        QString quizTime;
		QString teacherEmail;
        QVector<QPair<QString, QVector<QString>>> questions;
    };
    struct QuizInfo {
        QString quizId;
        QString quizName;
        QString teacherName;
        QString quizTime;
    };
    Quiz currentQuiz;

    QVector<QuizInfo> currentQuizzes;
    int currentQuestionIndex = 0;
    QVector<int> userAnswers;
    QButtonGroup* optionsGroup = nullptr;

    QString studentName;
	QString studentEmail;
    void setupWelcomePage();
    void setupQuizListPage();
    void setupQuizPage();
    void setupResultsPage();
    void displayQuestion();
    void updateQuizListUI(const QVector<QuizInfo>& quizzes);
    void clearQuizListUI();

private slots:
    void fetchQuizzesFromFirebase();
    void nextQuestion();
    void previousQuestion();
    void saveProgress();
    void submitQuiz();
    void showResults();
    void goToHomePage();
    void startQuiz();
    void checkQuizAttempted();
	void sendMarkstoFirebase(int marks, int totalMarks);
};

#endif // STUDENTPORTAL_H