#include "worker.h"

void Worker::doWork() {
    for (int i = 0; i <= 100; ++i) {
        if (QThread::currentThread()->isInterruptionRequested()) {
            emit progressChanged(0);
            return;
        }
        QThread::msleep(50); // 模拟耗时操作
        emit progressChanged(i);
    }
}
