#include "quizapp.h"
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>
quizapp::quizapp(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	ui.stackedWidget->setCurrentIndex(0);
    this->showMaximized();
	connect(ui.answer_text, &QLineEdit::returnPressed, this, &quizapp::handleInput);
	networkManager = new QNetworkAccessManager(this);
	networkReply = networkManager->get(QNetworkRequest(QUrl("https://aazmaish-quizapp-default-rtdb.asia-southeast1.firebasedatabase.app/Quizzes.json")));
	connect(networkReply, &QNetworkReply::finished, this, &quizapp::on_networkReplyFinished);
}

quizapp::~quizapp()
{
	networkManager->deleteLater();
}
void quizapp::on_login_btn_clicked()
{
	ui.stackedWidget->setCurrentIndex(1);
}
void quizapp::on_back_btn_clicked()
{
	ui.stackedWidget->setCurrentIndex(0);
}
void quizapp::on_back_btn_2_clicked()
{
	ui.stackedWidget->setCurrentIndex(1);
}

void quizapp::on_submit_btn_clicked()
{
	handleAnswer();
	ui.stackedWidget->setCurrentIndex(2);
}
void quizapp::handleAnswer()
{
	if (ui.answer_B->isChecked())
	{
		ui.answer_label->setText("Your answer is: Correct!");
	}
	else
	{
		ui.answer_label->setText("Your answer is: Incorrect!");
	}
}
void quizapp::handleInput()
{
	QString text = ui.answer_text->text();
    QJsonObject json;
	json["q3"] = text;
	QJsonDocument jsonDoc(json);
	QByteArray data = jsonDoc.toJson();
	QNetworkRequest request(QUrl("https://aazmaish-quizapp-default-rtdb.asia-southeast1.firebasedatabase.app/Quizzes/quiz1.json"));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	networkReply = networkManager->post(request, data);
	connect(networkReply, &QNetworkReply::finished, this, &quizapp::on_networkReplyFinished);
	ui.answer_text->clear();
	
}

void quizapp::on_networkReplyFinished()
{
    if (networkReply->error())
    {
        qDebug() << "Network Error:" << networkReply->errorString();
        networkReply->deleteLater();
        return;
    }

    QByteArray response_data = networkReply->readAll();
    qDebug() << "Response data:" << response_data;

    // Parse JSON data
    QJsonDocument jsonDoc = QJsonDocument::fromJson(response_data);
    if (jsonDoc.isNull() || !jsonDoc.isObject())
    {
        qDebug() << "Failed to parse JSON.";
        networkReply->deleteLater();
        return;
    }

    QJsonObject rootObj = jsonDoc.object();

    // Access "quiz1" object
    if (rootObj.contains("quiz1") && rootObj.value("quiz1").isObject())
    {
        QJsonObject quizObj = rootObj.value("quiz1").toObject();

        // Iterate over the questions in "quiz1"
        QStringList questionsList;
        foreach(const QString & key, quizObj.keys())
        {
            QString questionText = quizObj.value(key).toString();
            questionsList.append(questionText);
            qDebug() << key << ":" << questionText;
        }

        // Example: Display the first question in the UI
        if (!questionsList.isEmpty())
        {
            ui.reply->setText(questionsList.at(2) + "\n" + questionsList.at(3));
        }
    }
    else
    {
        qDebug() << "'quiz1' key not found or is not an object.";
    }

    networkReply->deleteLater();
}

