// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"

// 包含所有需要的头文件
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("天气查询工具");

    // --- 1. 初始化网络管理器 ---
    networkManager = new QNetworkAccessManager(this);

    // --- 2. 设置UI界面 ---
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    QHBoxLayout *inputLayout = new QHBoxLayout();
    cityInput = new QLineEdit();
    cityInput->setPlaceholderText("输入城市名 (拼音, 如 beijing)");
    searchButton = new QPushButton("查询");
    inputLayout->addWidget(new QLabel("城市:"));
    inputLayout->addWidget(cityInput);
    inputLayout->addWidget(searchButton);

    resultDisplay = new QTextEdit();
    resultDisplay->setReadOnly(true);

    mainLayout->addLayout(inputLayout);
    mainLayout->addWidget(resultDisplay);

    setCentralWidget(centralWidget);

    // --- 3. 连接信号与槽 ---
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::onSearchClicked);
}

void MainWindow::onSearchClicked()
{
    QString city = cityInput->text().trimmed();
    if (city.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入城市名！");
        return;
    }

    resultDisplay->setText("正在查询中，请稍候...");

    // 构造请求URL
    QUrl url("https://wttr.in/" + city + "?format=j1");
    QNetworkRequest request(url);

    // 发送GET请求，这是一个异步操作
    QNetworkReply *reply = networkManager->get(request);

    // 连接finished信号，当请求完成时，我们的槽函数会被调用
    connect(reply, &QNetworkReply::finished, this, [=](){
        onReplyFinished(reply);
    });
}

void MainWindow::onReplyFinished(QNetworkReply *reply)
{
    // 检查是否有网络错误
    if (reply->error() != QNetworkReply::NoError) {
        resultDisplay->setText("查询失败: " + reply->errorString());
    } else {
        // 读取所有返回数据
        QByteArray responseData = reply->readAll();

        // 解析JSON
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        if (jsonDoc.isNull()) {
            resultDisplay->setText("JSON解析失败！");
        } else {
            QJsonObject jsonObj = jsonDoc.object();
            // 根据wttr.in的JSON结构提取信息
            QJsonObject currentCondition = jsonObj["current_condition"].toArray()[0].toObject();
            QString tempC = currentCondition["temp_C"].toString();
            QString feelsLikeC = currentCondition["FeelsLikeC"].toString();
            QString humidity = currentCondition["humidity"].toString();
            QString weatherDesc = currentCondition["weatherDesc"].toArray()[0].toObject()["value"].toString();

            QString result = QString("温度: %1 °C\n体感温度: %2 °C\n湿度: %3%\n天气: %4")
                               .arg(tempC, feelsLikeC, humidity, weatherDesc);
            resultDisplay->setText(result);
        }
    }

    // **非常重要：释放reply对象，防止内存泄漏**
    reply->deleteLater();
}

MainWindow::~MainWindow()
{
    delete ui;
}
