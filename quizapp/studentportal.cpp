#include "StudentPortal.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

StudentPortal::StudentPortal(QObject* parent, Ui::quizappClass* uiPtr)
    : QObject(parent), ui(uiPtr) {
    manager = new QNetworkAccessManager(this);

    setupWelcomePage();
    setupQuizListPage();
    setupQuizPage();
    setupResultsPage();

}

StudentPortal::~StudentPortal() {

}

void StudentPortal::setupWelcomePage() {
    connect(ui->showQuizzesButton, &QPushButton::clicked, this, &StudentPortal::fetchQuizzesFromFirebase);
    connect(ui->logOutButton, &QPushButton::clicked, this, [this]() {
		ui->stackedWidget->setCurrentWidget(ui->welcomePage);
		ui->student_name_label->setText("Loading...");
		ui->student_email_label->clear();
		this->~StudentPortal();
        });

    QFont welcomeFont = ui->welcomeLabel->font();
    welcomeFont.setPointSize(24);
    ui->welcomeLabel->setFont(welcomeFont);
}

void StudentPortal::setupQuizListPage() {
    connect(ui->homeButton1, &QPushButton::clicked, [this]() {
        ui->stackedWidget->setCurrentWidget(ui->studentPortal);
        });
}

void StudentPortal::setupQuizPage() {
    optionsGroup = new QButtonGroup(this);
    optionsGroup->addButton(ui->optionA, 0);
    optionsGroup->addButton(ui->optionB, 1);
    optionsGroup->addButton(ui->optionC, 2);
    optionsGroup->addButton(ui->optionD, 3);

    connect(ui->nextButton, &QPushButton::clicked, this, &StudentPortal::nextQuestion);
    connect(ui->previousButton, &QPushButton::clicked, this, &StudentPortal::previousQuestion);
    connect(ui->saveButton, &QPushButton::clicked, this, &StudentPortal::saveProgress);
    connect(ui->submitQuizButton, &QPushButton::clicked, this, &StudentPortal::submitQuiz);
}

void StudentPortal::setupResultsPage() {
    connect(ui->homePageButton, &QPushButton::clicked, this, &StudentPortal::goToHomePage);

    QFont titleFont = ui->resultsLabel->font();
    titleFont.setPointSize(20);
    ui->resultsLabel->setFont(titleFont);
}

void StudentPortal::fetchQuizzesFromFirebase() {
    QUrl url("https://aazmaish-quizapp-default-rtdb.asia-southeast1.firebasedatabase.app/Quizzes.json");
    QNetworkRequest request(url);
    QNetworkReply* reply = manager->get(request);



    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
            if (jsonDoc.isObject()) {
                QJsonObject rootObj = jsonDoc.object();
                QVector<QuizInfo> quizzes;
                for (auto it = rootObj.begin(); it != rootObj.end(); ++it) {
                    QJsonObject qObj = it.value().toObject();
                    QuizInfo qi;
                    qi.quizId = it.key();
                    qi.quizName = qObj["quizName"].toString();
                    qi.teacherName = qObj["teacherName"].toString();
                    qi.quizTime = qObj["quizTime"].toString();
                    quizzes.append(qi);
                }
                updateQuizListUI(quizzes);
                ui->stackedWidget->setCurrentWidget(ui->quizListPage);
            }
        }
        else {
            QMessageBox::critical(nullptr, "Network Error", reply->errorString());
        }
        reply->deleteLater();
        });
}

void StudentPortal::updateQuizListUI(const QVector<QuizInfo>& quizzes) {
    currentQuizzes = quizzes;
    clearQuizListUI();

    for (const auto& quiz : quizzes) {
        QWidget* container = new QWidget;
        QLabel* quizNameLabel = new QLabel("<b>" + quiz.quizName + "</b>", container);
        quizNameLabel->setStyleSheet("font-size: 20px;");

        QLabel* teacherLabel = new QLabel("Teacher: " + quiz.teacherName, container);
        teacherLabel->setStyleSheet("font-size: 16px;");

        QLabel* startTimeLabel = new QLabel("Start time: " + quiz.quizTime, container);
        startTimeLabel->setStyleSheet("font-size: 16px;");

        QPushButton* startButton = new QPushButton("Start Quiz", container);
        startButton->setStyleSheet("background-color: #4CAF50; color: white;");
        startButton->setProperty("quizId", quiz.quizId);

        connect(startButton, &QPushButton::clicked, this, &StudentPortal::startQuiz);

        QVBoxLayout* vlayout = new QVBoxLayout();
        vlayout->addWidget(quizNameLabel);
        vlayout->addWidget(teacherLabel);
        vlayout->addWidget(startTimeLabel);
        vlayout->addWidget(startButton, 0, Qt::AlignRight);

        
        container->setLayout(vlayout);

        ui->verticalLayout_scrollArea->addWidget(container);
    }
}

void StudentPortal::clearQuizListUI() {
    QLayoutItem* item;
    while ((item = ui->verticalLayout_scrollArea->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
}

void StudentPortal::startQuiz() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    QString quizId = btn->property("quizId").toString();
    const QString baseUrl = "https://aazmaish-quizapp-default-rtdb.asia-southeast1.firebasedatabase.app/Quizzes/";
    QNetworkRequest request(QUrl(baseUrl + quizId + ".json"));
    QNetworkReply* detReply = manager->get(request);

    connect(detReply, &QNetworkReply::finished, this, [this, detReply]() {
        if (detReply->error() == QNetworkReply::NoError) {
            QByteArray data = detReply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(data);
            if (doc.isObject()) {
                QJsonObject quizObj = doc.object();
                currentQuiz.quizName = quizObj["quizName"].toString();
                currentQuiz.teacherName = quizObj["teacherName"].toString();
                currentQuiz.quizTime = quizObj["quizTime"].toString();
				currentQuiz.teacherEmail = quizObj["teacherEmail"].toString();
                currentQuiz.questions.clear();
                userAnswers.clear();

                QJsonArray questionsArray = quizObj["questions"].toArray();
                for (const QJsonValue& val : questionsArray) {
                    QJsonObject qObj = val.toObject();
                    QString questionText = qObj["question"].toString();
                    QString optionA = qObj["optionA"].toString();
					QString optionB = qObj["optionB"].toString();
					QString optionC = qObj["optionC"].toString();
					QString optionD = qObj["optionD"].toString();
					QString correctOption = qObj["correctOption"].toString();

                    QVector<QString> opts;
					opts.push_back(optionA);
					opts.push_back(optionB);
					opts.push_back(optionC);
					opts.push_back(optionD);
					opts.push_back(correctOption);
                    currentQuiz.questions.append(qMakePair(questionText, opts));
                    userAnswers.append(-1);
                }

                currentQuestionIndex = 0;
                displayQuestion();
                ui->stackedWidget->setCurrentWidget(ui->quizPage);
            }
        }
        else {
            QMessageBox::critical(nullptr, "Network Error", detReply->errorString());
        }
        detReply->deleteLater();
        });
}

void StudentPortal::displayQuestion() {
    if (currentQuestionIndex < 0 || currentQuestionIndex >= currentQuiz.questions.size())
        return;

    ui->quizName->setText(currentQuiz.quizName);
    ui->question->setText(currentQuiz.questions[currentQuestionIndex].first);
	ui->question_number->setText("Question " + QString::number(currentQuestionIndex + 1));
	ui->quizName->setAlignment(Qt::AlignCenter);
	ui->question->setAlignment(Qt::AlignCenter);
	ui->question_number->setAlignment(Qt::AlignCenter);
	ui->question->setStyleSheet("font-size: 18px;");
	ui->question_number->setStyleSheet("font-size: 20px;");
    const QVector<QString> optionVec = currentQuiz.questions[currentQuestionIndex].second;
    if (optionVec.size() > 3) {
        ui->optionA->setText("A: " + optionVec[0]);
        ui->optionB->setText("B: " + optionVec[1]);
        ui->optionC->setText("C: " + optionVec[2]);
        ui->optionD->setText("D: " + optionVec[3]);
		ui->optionA->setStyleSheet("font-size: 16px;");
		ui->optionB->setStyleSheet("font-size: 16px;");
		ui->optionC->setStyleSheet("font-size: 16px;");
		ui->optionD->setStyleSheet("font-size: 16px;");
    }
    else {
        QMessageBox::critical(nullptr, "Data Error", "No options available for the current question.");
    }
    
    

    optionsGroup->setExclusive(false);
    ui->optionA->setChecked(false);
    ui->optionB->setChecked(false);
    ui->optionC->setChecked(false);
    ui->optionD->setChecked(false);
    optionsGroup->setExclusive(true);

    if (userAnswers[currentQuestionIndex] != -1) {
        QAbstractButton* btn = optionsGroup->button(userAnswers[currentQuestionIndex]);
        if (btn) btn->setChecked(true);
    }
}

void StudentPortal::nextQuestion() {
    saveProgress();
    if (currentQuestionIndex < currentQuiz.questions.size() - 1) {
        currentQuestionIndex++;
        displayQuestion();
    }
}

void StudentPortal::previousQuestion() {
    saveProgress();
    if (currentQuestionIndex > 0) {
        currentQuestionIndex--;
        displayQuestion();
    }
}

void StudentPortal::saveProgress() {
    int selectedOpt = optionsGroup->checkedId();
    if (selectedOpt != -1) {
        userAnswers[currentQuestionIndex] = selectedOpt;
    }
}

void StudentPortal::submitQuiz() {
    saveProgress();
    ui->stackedWidget->setCurrentWidget(ui->resultsPage);

    ui->resultsTable->setRowCount(currentQuiz.questions.size());
	ui->resultsTable->setHorizontalHeaderLabels(QStringList() << "Question" << "Your Answer" << "Correct Answer");
    marks = 0;
    int totalMarks = currentQuiz.questions.size();
    for (int i = 0; i < currentQuiz.questions.size(); ++i) {
        ui->resultsTable->setItem(i, 0, new QTableWidgetItem(currentQuiz.questions[i].first));
		QString correctOption = currentQuiz.questions[i].second[4];
        int correctOptionIndex = 0;
        if (correctOption == "A") {
            correctOptionIndex = 0;
        } else if (correctOption == "B") {
            correctOptionIndex = 1;
        } else if (correctOption == "C") {
            correctOptionIndex = 2;
        } else if (correctOption == "D") {
            correctOptionIndex = 3;
        }
        QString userString = "No answer";
        if (userAnswers[i] != -1) {
            userString = currentQuiz.questions[i].second[userAnswers[i]];
        }
        ui->resultsTable->setItem(i, 1, new QTableWidgetItem(userString));

        correctOption = currentQuiz.questions[i].second.isEmpty() ? "N/A" : currentQuiz.questions[i].second[correctOptionIndex];
        ui->resultsTable->setItem(i, 2, new QTableWidgetItem(correctOption));
        if (correctOptionIndex == userAnswers[i]) marks++;
    }
    ui->marks_label->setText("Your Marks: " + QString::number(marks) + "/" + QString::number(totalMarks));
	ui->marks_label->setAlignment(Qt::AlignCenter);
	ui->marks_label->setStyleSheet("font-size: 24px;");
	checkQuizAttempted();
}

void StudentPortal::showResults() {
    ui->stackedWidget->setCurrentWidget(ui->resultsPage);
}

void StudentPortal::goToHomePage() {
    ui->stackedWidget->setCurrentWidget(ui->studentPortal);
}

void StudentPortal::setIDToken(const QString& idToken_p) {
	idToken = idToken_p;
}

void StudentPortal::setStudentName(const QString& name) {
	studentName = name;
	ui->student_name_label->setText(studentName);
	ui->student_name_label->setAlignment(Qt::AlignCenter);
	ui->student_name_label->setStyleSheet("font-size: 24px;");
}
void StudentPortal::setStudentEmail(const QString& email) {
    studentEmail = email;
    ui->student_email_label->setText(studentEmail);
    ui->student_email_label->setAlignment(Qt::AlignCenter);
}

void StudentPortal::checkQuizAttempted() {
    //checking if user has already attempted the quiz, if yes then marks wont be uploaded.
    QUrl url("https://aazmaish-quizapp-default-rtdb.asia-southeast1.firebasedatabase.app/marks.json");
    QNetworkReply* networkReply;
	networkReply = manager->get(QNetworkRequest(url)); //downloading marks from firebase
	connect(networkReply, &QNetworkReply::finished, this, [this, networkReply]() {
		QByteArray response = networkReply->readAll();
		QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
		QJsonObject marksObj = jsonDoc.object();
		if (marksObj.contains("error")) {
			QJsonObject error = marksObj["error"].toObject();
			QMessageBox::warning(nullptr, "Error", error["message"].toString());
			return;
		}
		for (QString key : marksObj.keys()) {
			QJsonObject markObject = marksObj.value(key).toObject();
			QString email = studentEmail;
			if (markObject["studentEmail"].toString() == email && markObject["quizName"].toString() == currentQuiz.quizName
                && markObject["teacherEmail"].toString() == currentQuiz.teacherEmail) {
				QMessageBox::information(nullptr, "NO RESUBMISSION", "You have already attempted this quiz, your marks won't be reuploaded");
				networkReply->deleteLater();
				return;
			}
		}
		sendMarkstoFirebase(marks, currentQuiz.questions.size());
		networkReply->deleteLater();
		});
}
void StudentPortal::sendMarkstoFirebase(int marks, int totalMarks) {
	QNetworkRequest request(QUrl("https://aazmaish-quizapp-default-rtdb.asia-southeast1.firebasedatabase.app/marks.json"));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	QJsonObject markObject;
	markObject["studentEmail"] = studentEmail;
	markObject["studentName"] = studentName;
	markObject["quizName"] = currentQuiz.quizName;
	markObject["teacherEmail"] = currentQuiz.teacherEmail;
	markObject["marks"] = marks;
	markObject["totalMarks"] = totalMarks;
	QJsonDocument markDoc(markObject);
	QNetworkReply* networkReply = manager->post(request, markDoc.toJson());
	connect(networkReply, &QNetworkReply::finished, this, [this, networkReply]() {
		QByteArray response = networkReply->readAll();
		QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
		if (jsonDoc.object().contains("error")) {
			QJsonObject error = jsonDoc.object()["error"].toObject();
			QMessageBox::warning(nullptr, "Error", error["message"].toString());
			networkReply->deleteLater();
			return;
		}
		QMessageBox::information(nullptr, "Success", "Marks uploaded successfully");
		networkReply->deleteLater();
		});
}