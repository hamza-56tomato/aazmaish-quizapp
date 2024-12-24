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
	connect(ui->clear_quiz_inputs_btn, &QPushButton::clicked, this, &TeacherPortal::clear_quiz_inputs);

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
    
}
void TeacherPortal::on_signOut_btn_clicked() {
	clear_quiz_inputs();
    ui->stackedWidget->setCurrentWidget(ui->welcomePage);
	this->~TeacherPortal();
}
void TeacherPortal::on_create_quiz_back_btn_clicked() {
	ui->stackedWidget->setCurrentWidget(ui->teacherPortal);
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
        entry.question->clear();
		entry.optionA->clear();
        entry.optionB->clear();
        entry.optionC->clear();
		entry.optionD->clear();
        foreach(QAbstractButton* button, entry.correctAnswerGroup->buttons()) {
            button->setChecked(false); // Uncheck the button
        }
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
    ui->stackedWidget->setCurrentWidget(ui->teacherPortal);
    networkReply->deleteLater();
}

