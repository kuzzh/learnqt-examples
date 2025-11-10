#include "paintwidget.h" // 包含我们的新类
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PaintWidget w;
    w.setWindowTitle("简易画板");
    w.resize(500, 500);
    w.show();
    return a.exec();
}
