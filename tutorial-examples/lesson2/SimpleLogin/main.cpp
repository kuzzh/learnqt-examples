#include <QApplication>
#include <QWidget>

// --- 新增的头文件 ---
#include <QLabel>       // 标签
#include <QLineEdit>      // 单行输入框
#include <QPushButton>    // 按钮
#include <QMessageBox>    // 消息对话框
#include <QVBoxLayout>    // 垂直布局
#include <QHBoxLayout>    // 水平布局

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 创建一个主窗口
    QWidget window;
    window.setWindowTitle("信号与槽示例");

    // --- 创建界面控件 ---
    QLabel *userLabel = new QLabel("用户名:");
    QLineEdit *userLineEdit = new QLineEdit;
    QLabel *passLabel = new QLabel("密  码:");
    QLineEdit *passLineEdit = new QLineEdit;
    passLineEdit->setEchoMode(QLineEdit::Password); // 将密码框设置为密码模式

    QPushButton *loginButton = new QPushButton("登录");

    // --- 告别.move()，拥抱布局！ ---
    // 水平布局1：用于用户名
    QHBoxLayout *userLayout = new QHBoxLayout;
    userLayout->addWidget(userLabel);
    userLayout->addWidget(userLineEdit);

    // 水平布局2：用于密码
    QHBoxLayout *passLayout = new QHBoxLayout;
    passLayout->addWidget(passLabel);
    passLayout->addWidget(passLineEdit);

    // 垂直主布局：将上面两个水平布局和按钮垂直排列
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(userLayout);
    mainLayout->addLayout(passLayout);
    mainLayout->addWidget(loginButton);

    // 将主布局应用到窗口上
    window.setLayout(mainLayout);

    // --- 核心：连接信号与槽 ---
    QObject::connect(loginButton, &QPushButton::clicked, [&]() {
        // 这是C++11的Lambda表达式，一个匿名的“槽函数”
        QString username = userLineEdit->text();
        QString password = passLineEdit->text();
        QString message = QString("用户名: %1\n密码: %2").arg(username, password);

        QMessageBox::information(&window, "登录信息", message);
    });

    window.show();
    return a.exec();
}
