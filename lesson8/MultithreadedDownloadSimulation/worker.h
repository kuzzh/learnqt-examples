#ifndef WORKER_H
#define WORKER_H

#include <QThread>

// 创建 Worker 类
class Worker : public QObject {
    Q_OBJECT
public slots:
    void doWork();
signals:
    void progressChanged(int value);
};

#endif // WORKER_H
