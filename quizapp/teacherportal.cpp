#include "teacherportal.h"
#include <QGridLayout>
#include <QRadioButton>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QMessageBox>
#include <qDebug>
#include <QFont>
#include <QUrl>
#include <QUrlQuery>

TeacherPortal::TeacherPortal(QObject* parent, Ui::quizappClass* uiPtr)
    : QObject(parent), ui(uiPtr)
{
    questionCount = 1;
	networkManager = new QNetworkAccessManager(this);
    networkReply = nullptr;
    connect(ui->add_question_btn, &QPushButton::clicked, this, &TeacherPortal::addNewQuestion);
    connect(ui->create_quiz_btn, &QPushButton::clicked, this, &TeacherPortal::on_create_quiz_btn_clicked);
    connect(ui->submit_quiz_btn, &QPushButton::clicked, this, &TeacherPortal::on_submit_quiz_button_clicked);
    connect(ui->signOut_btn, &QPushButton::clicked, this, &TeacherPortal::on_signOut_btn_clicked);
	connect(ui->create_quiz_back_btn, &QPushButton::clicked, this, &TeacherPortal::on_create_quiz_back_btn_clicked);
	connect(ui->edit_quiz_back_btn, &QPushButton::clicked, this, &TeacherPortal::on_edit_quiz_back_btn_clicked);
	connect(ui->clear_quiz_inputs_btn, &QPushButton::clicked, this, &TeacherPortal::clear_quiz_inputs);
	connect(ui->edit_quiz_btn, &QPushButton::clicked, this, &TeacherPortal::on_edit_quiz_btn_clicked);
	connect(ui->view_marks_btn, &QPushButton::clicked, this, &TeacherPortal::on_view_marks_btn_clicked);
	connect(ui->view_marks_back_btn, &QPushButton::clicked, this, &TeacherPortal::on_view_marks_back_btn_clicked);
	connect(ui->marks_back_btn, &QPushButton::clicked, this, &TeacherPortal::on_marks_back_btn_clicked);

    QuestionEntry entry;
    QButtonGroup* correctAnswerGroup = new QButtonGroup(this);
    correctAnswerGroup->addButton(ui->a_radio, 0);
    correctAnswerGroup->addButton(ui->b_radio, 1);
    correctAnswerGroup->addButton(ui->c_radio, 2);
    correctAnswerGroup->addButton(ui->d_radio, 3);
    entry.question = ui->question1_input;
    entry.optionA = ui->a_input;
    entry.optionB = ui->b_input;
    entry.optionC = ui->c_input;
    entry.optionD = ui->d_input;
    entry.correctAnswerGroup = correctAnswerGroup;
    questionsList.append(entry);
}
TeacherPortal::~TeacherPortal() {
	networkManager->deleteLater();
}
void TeacherPortal::on_signOut_btn_clicked() {
	clear_quiz_inputs();
    ui->stackedWidget->setCurrentWidget(ui->welcomePage);
    ui->teacher_name_label->setText("Loading...");
    ui->teacher_email_label->clear();
	this->~TeacherPortal();
}
void TeacherPortal::on_create_quiz_back_btn_clicked() {
	ui->stackedWidget->setCurrentWidget(ui->teacherPortal);
}
void TeacherPortal::on_edit_quiz_back_btn_clicked() {
	ui->stackedWidget->setCurrentWidget(ui->teacherPortal);
    QLayoutItem* item;
    while ((item = ui->edit_quizzes_list->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
	quizzesList.clear();
}
void TeacherPortal::setIDToken(const QString& idToken_p) {
	idToken = idToken_p;
}
void TeacherPortal::on_create_quiz_btn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->createQuizPage);
	questionCount = 1;

}

void TeacherPortal::addNewQuestion()
{
    QGridLayout* gridLayout = new QGridLayout();

    QWidget* container = new QWidget;
    container->setLayout(gridLayout);

    QLabel* questionLabel = new QLabel(QString("Question %1").arg(++questionCount), container);
    QLabel* labelA = new QLabel("A", container);
    QLabel* labelB = new QLabel("B", container);
    QLabel* labelC = new QLabel("C", container);
    QLabel* labelD = new QLabel("D", container);
    QLineEdit* question = new QLineEdit(container);
    QLineEdit* optionA = new QLineEdit(container);
    QLineEdit* optionB = new QLineEdit(container);
    QLineEdit* optionC = new QLineEdit(container);
    QLineEdit* optionD = new QLineEdit(container);
    QLabel* correctAnswerLabel = new QLabel("Correct", container);

    QRadioButton* radioA = new QRadioButton("A", container);
    QRadioButton* radioB = new QRadioButton("B", container);
    QRadioButton* radioC = new QRadioButton("C", container);
    QRadioButton* radioD = new QRadioButton("D", container);

    QButtonGroup* correctAnswerGroup = new QButtonGroup(container);
    correctAnswerGroup->addButton(radioA, 0);
    correctAnswerGroup->addButton(radioB, 1);
    correctAnswerGroup->addButton(radioC, 2);
    correctAnswerGroup->addButton(radioD, 3);

    QGridLayout* radioLayout = new QGridLayout();
    radioLayout->addWidget(radioA, 0, 0);
    radioLayout->addWidget(radioB, 0, 1);
    radioLayout->addWidget(radioC, 0, 2);
    radioLayout->addWidget(radioD, 0, 3);

    QWidget* radioWidget = new QWidget(container);
    radioWidget->setLayout(radioLayout);

    // Set styles
    optionA->setStyleSheet("background: white;");
    optionB->setStyleSheet("background: white;");
    optionC->setStyleSheet("background: white;");
    optionD->setStyleSheet("background: white;");
    question->setStyleSheet("background: white;");

    // Add widgets to the grid layout
    gridLayout->addWidget(questionLabel, 0, 0);
    gridLayout->addWidget(question, 0, 1);
    gridLayout->addWidget(labelA, 1, 0);
    gridLayout->addWidget(optionA, 1, 1);
    gridLayout->addWidget(labelB, 2, 0);
    gridLayout->addWidget(optionB, 2, 1);
    gridLayout->addWidget(labelC, 3, 0);
    gridLayout->addWidget(optionC, 3, 1);
    gridLayout->addWidget(labelD, 4, 0);
    gridLayout->addWidget(optionD, 4, 1);
    gridLayout->addWidget(correctAnswerLabel, 5, 0);
    gridLayout->addWidget(radioWidget, 5, 1);

    // Add the container to the main layout
    ui->questions_layout->addWidget(container);

    // Store the widgets in QuestionEntry and add to the list
    QuestionEntry entry;
    entry.question = question;
    entry.optionA = optionA;
    entry.optionB = optionB;
    entry.optionC = optionC;
    entry.optionD = optionD;
    entry.correctAnswerGroup = correctAnswerGroup;

    questionsList.append(entry);
}

void TeacherPortal::clear_quiz_inputs() {
    ui->quiz_name_input->clear();
    ui->quiz_time_input->clear();
	QLayoutItem* item;
	while ((item = ui->questions_layout->takeAt(1)) != nullptr) {
		delete item->widget();
		delete item;
	}
    questionCount = 1;
	questionsList.clear();
	ui->question1_input->clear();
	ui->a_input->clear();
	ui->b_input->clear();
	ui->c_input->clear();
	ui->d_input->clear();
	
    QuestionEntry entry;
    QButtonGroup* correctAnswerGroup = new QButtonGroup(this);
    correctAnswerGroup->addButton(ui->a_radio, 0);
    correctAnswerGroup->addButton(ui->b_radio, 1);
    correctAnswerGroup->addButton(ui->c_radio, 2);
    correctAnswerGroup->addButton(ui->d_radio, 3);
    entry.question = ui->question1_input;
    entry.optionA = ui->a_input;
    entry.optionB = ui->b_input;
    entry.optionC = ui->c_input;
    entry.optionD = ui->d_input;
    entry.correctAnswerGroup = correctAnswerGroup;

    questionsList.append(entry);
}
void TeacherPortal::on_submit_quiz_button_clicked()
{
    QJsonArray questionArray;

    for (const QuestionEntry& entry : questionsList)
    {
        QString questionText = entry.question->text();
        QString optionA = entry.optionA->text();
        QString optionB = entry.optionB->text();
        QString optionC = entry.optionC->text();
        QString optionD = entry.optionD->text();

        int correctOptionId = entry.correctAnswerGroup->checkedId();
        QString correctOption;
        switch (correctOptionId)
        {
        case 0:
            correctOption = "A";
            break;
        case 1:
            correctOption = "B";
            break;
        case 2:
            correctOption = "C";
            break;
        case 3:
            correctOption = "D";
            break;
        default:
            correctOption = "";
            QMessageBox::warning(nullptr, "Error", "Please select a correct option for each question");
            return;
        }

        QJsonObject questionObject;
        questionObject["question"] = questionText;
        questionObject["optionA"] = optionA;
        questionObject["optionB"] = optionB;
        questionObject["optionC"] = optionC;
        questionObject["optionD"] = optionD;
        questionObject["correctOption"] = correctOption;

        questionArray.append(questionObject);
    }

    QJsonObject quizObject;
	quizObject["quizName"] = ui->quiz_name_input->text();
	quizObject["quizTime"] = ui->quiz_time_input->text();
    quizObject["questions"] = questionArray;
	quizObject["teacherName"] = teacherName;
	quizObject["teacherEmail"] = teacherEmail;

    QJsonDocument jsonDoc(quizObject);

	networkReply = networkManager->post(QNetworkRequest(QUrl("https://aazmaish-quizapp-default-rtdb.asia-southeast1.firebasedatabase.app/Quizzes.json")), jsonDoc.toJson());
	connect(networkReply, &QNetworkReply::finished, this, &TeacherPortal::on_create_quiz_reply_finished);
    
}
void TeacherPortal::on_create_quiz_reply_finished() {
    QByteArray response = networkReply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
    if (jsonDoc.object().contains("error")) {
        QJsonObject error = jsonDoc.object()["error"].toObject();
        QMessageBox::warning(nullptr, "Error", error["message"].toString());
        return;
    }
    QMessageBox::information(nullptr, "Success", "Quiz created successfully");
	clear_quiz_inputs();
    ui->stackedWidget->setCurrentWidget(ui->teacherPortal);
    networkReply->deleteLater();
}

void TeacherPortal::setTeacherName(const QString& name) {
	teacherName = name;

	ui->teacher_name_label->setText(teacherName);
    QFont f;
	f.setPointSize(36);
	ui->teacher_name_label->setFont(f);
	ui->teacher_name_label->setAlignment(Qt::AlignCenter);
}

void TeacherPortal::setTeacherEmail(const QString& email) {
	teacherEmail = email;
	ui->teacher_email_label->setText(teacherEmail);
	ui->teacher_email_label->setAlignment(Qt::AlignCenter);
}

void TeacherPortal::on_edit_quiz_btn_clicked() {
    QUrl url("https://aazmaish-quizapp-default-rtdb.asia-southeast1.firebasedatabase.app/Quizzes.json");
    networkReply = networkManager->get(QNetworkRequest(url));
    connect(networkReply, &QNetworkReply::finished, this, [this]() {
        ui->stackedWidget->setCurrentWidget(ui->editQuizPage);
        QByteArray response = networkReply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
        if (jsonDoc.object().contains("error")) {
            qDebug() << jsonDoc.object();
            QJsonObject error = jsonDoc.object()["error"].toObject();
            QMessageBox::warning(nullptr, "Error", error["message"].toString());
            return;
        }
        QJsonObject quizzes = jsonDoc.object();
        quizzesList.clear();
        for (const QString& key : quizzes.keys()) {
            QJsonObject quiz = quizzes[key].toObject();
            // Check if the teacherEmail matches
            if (quiz["teacherEmail"].toString() == teacherEmail) {
                QuizzesData data;
                data.quizName = quiz["quizName"].toString();
                data.quizTime = quiz["quizTime"].toString();
                data.quizID = key;
                quizzesList.append(data);
            }
        }
        generate_quizzes_to_edit();
        networkReply->deleteLater();
        });
}

void TeacherPortal::generate_quizzes_to_edit() {
	//generate widgets for each quiz
	for (const QuizzesData& quiz : quizzesList) {
        QWidget* container = new QWidget;
        QGridLayout* gridLayout = new QGridLayout();
        gridLayout->setHorizontalSpacing(20);
        QLabel* label1 = new QLabel(quiz.quizName);
        QLabel* label2 = new QLabel(quiz.quizTime);
		label1->setStyleSheet("font-size: 24px;");
        container->setStyleSheet("background-color: azure;");
        gridLayout->addWidget(label1, 0, 0);
        gridLayout->addWidget(label2, 1, 0);
        QSpacerItem* horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        gridLayout->addItem(horizontalSpacer, 0, 2);
        QPushButton* deleteButton = new QPushButton("Delete Quiz");
        deleteButton->setStyleSheet("background-color: #ff474c;font-weight: bold;");
        gridLayout->addWidget(deleteButton, 0, 3, 2, 1);
        container->setLayout(gridLayout);
        ui->edit_quizzes_list->addWidget(container);
		connect(deleteButton, &QPushButton::clicked, this, [this, quiz, container]() {
			QUrl url(QString("https://aazmaish-quizapp-default-rtdb.asia-southeast1.firebasedatabase.app/Quizzes/%1.json").arg(quiz.quizID));
			networkReply = networkManager->deleteResource(QNetworkRequest(url));
			connect(networkReply, &QNetworkReply::finished, this, [this, container]() {
				QByteArray response = networkReply->readAll();
				QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
				if (jsonDoc.object().contains("error")) {
					QJsonObject error = jsonDoc.object()["error"].toObject();
					QMessageBox::warning(nullptr, "Error", error["message"].toString());
					return;
				}
				QMessageBox::information(nullptr, "Success", "Quiz deleted successfully");
				networkReply->deleteLater();
				ui->edit_quizzes_list->removeWidget(container);
                delete container;
				});
			});
	}
	if (quizzesList.empty()) {
		QLabel* label = new QLabel("No quizzes found");
		label->setStyleSheet("font-size: 24px;");
		ui->edit_quizzes_list->addWidget(label);
	}
}

void TeacherPortal::on_view_marks_btn_clicked() {
    QUrl url("https://aazmaish-quizapp-default-rtdb.asia-southeast1.firebasedatabase.app/Quizzes.json");
    networkReply = networkManager->get(QNetworkRequest(url));
    connect(networkReply, &QNetworkReply::finished, this, [this]() {
        ui->stackedWidget->setCurrentWidget(ui->viewMarksPage);
        QByteArray response = networkReply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
        if (jsonDoc.object().contains("error")) {
            qDebug() << jsonDoc.object();
            QJsonObject error = jsonDoc.object()["error"].toObject();
            QMessageBox::warning(nullptr, "Error", error["message"].toString());
            return;
        }
        QJsonObject quizzes = jsonDoc.object();
        quizzesList.clear();
        for (const QString& key : quizzes.keys()) {
            QJsonObject quiz = quizzes[key].toObject();
            // Check if the teacherEmail matches
            if (quiz["teacherEmail"].toString() == teacherEmail) {
                QuizzesData data;
                data.quizName = quiz["quizName"].toString();
                data.quizTime = quiz["quizTime"].toString();
                data.quizID = key;
                quizzesList.append(data);
            }
        }
        generate_quizzes_to_view_marks();
		networkReply->deleteLater();
	});
}
void TeacherPortal::generate_quizzes_to_view_marks() {
    //generate widgets for each quiz
    for (const QuizzesData& quiz : quizzesList) {
        QWidget* container = new QWidget;
        QGridLayout* gridLayout = new QGridLayout();
        gridLayout->setHorizontalSpacing(20);
        QLabel* label1 = new QLabel(quiz.quizName);
        QLabel* label2 = new QLabel(quiz.quizTime);
        label1->setStyleSheet("font-size: 24px;");
        container->setStyleSheet("background-color: azure;");
        gridLayout->addWidget(label1, 0, 0);
        gridLayout->addWidget(label2, 1, 0);
        QSpacerItem* horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        gridLayout->addItem(horizontalSpacer, 0, 2);
        QPushButton* viewMarksButton = new QPushButton("View Marks");
        viewMarksButton->setStyleSheet("background-color: #ababab;");
        gridLayout->addWidget(viewMarksButton, 0, 3, 2, 1);
        container->setLayout(gridLayout);
        ui->quizzesList_marks->addWidget(container);
        connect(viewMarksButton, &QPushButton::clicked, this, [this, quiz]() {
            QUrl url("https://aazmaish-quizapp-default-rtdb.asia-southeast1.firebasedatabase.app/marks.json");
			networkReply = networkManager->get(QNetworkRequest(url));
            connect(networkReply, &QNetworkReply::finished, this, [this, quiz]() {
                QByteArray response = networkReply->readAll();
                QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                if (jsonDoc.object().contains("error")) {
                    QJsonObject error = jsonDoc.object()["error"].toObject();
                    QMessageBox::warning(nullptr, "Error", error["message"].toString());
                    return;
                }
				QJsonObject marksObj = jsonDoc.object();
				for (QString key : marksObj.keys()) {
					QJsonObject markObject = marksObj.value(key).toObject();
					if (markObject["quizName"].toString() == quiz.quizName && markObject["teacherEmail"].toString() == teacherEmail) {
						int marks = markObject["marks"].toInt();
						int totalMarks = markObject["totalMarks"].toInt();
                        qDebug() << marks << totalMarks;
						QString studentEmail = markObject["studentEmail"].toString();
						QString studentName = markObject["studentName"].toString();
						generate_student_marks(QString::number(marks), QString::number(totalMarks), studentName, studentEmail);
					}
				}
				if (ui->marks_list->count() == 0) {
					QLabel* label = new QLabel("No student has attempted this quiz yet");
					label->setStyleSheet("font-size: 24px;");
					ui->marks_list->addWidget(label);
				}
                ui->stackedWidget->setCurrentWidget(ui->marksPage);
				networkReply->deleteLater();
             });
        });
    }
	if (quizzesList.empty()) {
		QLabel* label = new QLabel("No quizzes found");
		label->setStyleSheet("font-size: 24px;");
		ui->quizzesList_marks->addWidget(label);
	}
}
void TeacherPortal::generate_student_marks(const QString& marks, const QString& totalMarks, const QString& studentName, const QString& studentEmail) {
	QWidget* container = new QWidget;
	QGridLayout* gridLayout = new QGridLayout();
	gridLayout->setHorizontalSpacing(20);
	QLabel* label1 = new QLabel("Name: " + studentName);
	QLabel* label2 = new QLabel("Email: " + studentEmail);
	QLabel* label3 = new QLabel("Marks: " + marks + "/" + totalMarks);
	label1->setStyleSheet("font-size: 20px;");
    label2->setStyleSheet("font-size: 20px;");
	label3->setStyleSheet("font-size: 20px;");
	container->setStyleSheet("background-color: azure;");
	gridLayout->addWidget(label1, 0, 0);
	gridLayout->addWidget(label2, 1, 0);
	gridLayout->addWidget(label3, 2, 0);
	container->setLayout(gridLayout);
	ui->marks_list->addWidget(container);
}
void TeacherPortal::on_view_marks_back_btn_clicked() {
	ui->stackedWidget->setCurrentWidget(ui->teacherPortal);
    QLayoutItem* item;
    while ((item = ui->quizzesList_marks->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    quizzesList.clear();
}

void TeacherPortal::on_marks_back_btn_clicked() {
	ui->stackedWidget->setCurrentWidget(ui->viewMarksPage);
    QLayoutItem* item;
    while ((item = ui->marks_list->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
}
