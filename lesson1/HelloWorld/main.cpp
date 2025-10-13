// 包含我们需要的三个头文件
#include <QApplication> // 包含应用程序类的定义
#include <QWidget>      // 包含窗口控件的定义
#include <QLabel>       // 包含标签控件的定义

int main(int argc, char *argv[])
{
    // 1. 创建一个QApplication对象，这是每个Qt GUI程序的“灵魂”
    QApplication a(argc, argv);

    // 2. 创建一个QWidget对象，它是一个空白的窗口
    QWidget window;
    window.setWindowTitle("我的第一个Qt程序"); // 设置窗口标题
    window.resize(400, 300);                  // 设置窗口的初始大小

    // 3. 创建一个QLabel对象（标签），用于显示文本
    //    构造函数的第二个参数 &window 表示这个标签的“父窗口”是window
    //    当window被销毁时，label也会被自动销毁，这是Qt重要的父子对象机制
    QLabel label("你好, Qt! 👋", &window);
    label.move(150, 140); // 把标签移动到窗口中的 (150, 140) 坐标

    // 4. 让窗口显示出来
    window.show();

    // 5. 启动应用程序的事件循环
    //    程序会在这里等待用户的操作（如点击、键盘输入等）
    return a.exec();
}
