#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QTextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots: // 声明私有槽函数
    void openFile();
    void saveFile();

private:
    Ui::MainWindow *ui;
    QTextEdit *textEdit; // 将textEdit提升为成员变量，方便槽函数访问
};
#endif // MAINWINDOW_H
