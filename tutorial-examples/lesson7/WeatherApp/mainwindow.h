// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

// 前向声明，减少头文件依赖
class QLineEdit;
class QPushButton;
class QTextEdit;
class QNetworkAccessManager;
class QNetworkReply;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSearchClicked();       // “查询”按钮点击槽
    void onReplyFinished(QNetworkReply *reply); // 网络回复处理槽

private:
    Ui::MainWindow *ui;

    // UI 控件
    QLineEdit *cityInput;
    QPushButton *searchButton;
    QTextEdit *resultDisplay;

    // 网络核心类
    QNetworkAccessManager *networkManager;
};
#endif // MAINWINDOW_H
