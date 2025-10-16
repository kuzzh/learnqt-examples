// main.cpp
#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QDebug>
#include "worker.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWindow window;
    window.setWindowTitle("多线程下载模拟");

    // 1. 创建UI
    QWidget *centralWidget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    QPushButton *startButton = new QPushButton("开始下载");
    QProgressBar *progressBar = new QProgressBar();
    layout->addWidget(startButton);
    layout->addWidget(progressBar);
    window.setCentralWidget(centralWidget);

    // 2. 创建线程和Worker
    QThread *thread = new QThread(&window);
    Worker *worker = new Worker();
    worker->moveToThread(thread);

    // 3. 连接信号与槽
    // 点击按钮，启动线程，进而开始工作
    QObject::connect(startButton, &QPushButton::clicked, thread, [=](bool) {
        thread->start();
    });
    QObject::connect(thread, &QThread::started, worker, &Worker::doWork);

    // Worker报告进度，UI更新进度条
    QObject::connect(worker, &Worker::progressChanged, progressBar, &QProgressBar::setValue);

    // 工作完成，线程退出
    QObject::connect(worker, &Worker::progressChanged, worker, [&](int v){ if(v==100) thread->quit(); });

    // 线程结束后，清理资源
    QObject::connect(thread, &QThread::finished, worker, &Worker::deleteLater);
    QObject::connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    window.show();
    return a.exec();
}
