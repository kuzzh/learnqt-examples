// mainwindow.cpp

#include "mainwindow.h"
#include "ui_mainwindow.h"

// --- 新增的头文件 ---
#include <QAction>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // --- 我们将在这里添加自己的代码 ---

    // 1. 创建一个QTextEdit作为中心控件
    QTextEdit *textEdit = new QTextEdit(this);
    setCentralWidget(textEdit);

    // 2. 创建Action (动作)，它是菜单项和工具栏按钮的“灵魂”
    QAction *newAction = new QAction("新建", this);
    QAction *openAction = new QAction("打开", this);
    QAction *exitAction = new QAction("退出", this);

    // (可选) 为Action添加快捷键
    exitAction->setShortcut(QKeySequence("Ctrl+Q"));

    // 3. 创建菜单栏，并添加菜单和Action
    QMenu *fileMenu = menuBar()->addMenu("文件(&F)"); // &F 设置伙伴快捷键 Alt+F
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addSeparator(); // 添加一条分割线
    fileMenu->addAction(exitAction);

    // 4. 创建工具栏，并添加Action
    QToolBar *fileToolBar = addToolBar("文件");
    fileToolBar->addAction(newAction);
    fileToolBar->addAction(openAction);

    // 5. 创建状态栏
    statusBar()->showMessage("准备就绪...");

    // 6. 连接信号与槽
    // 点击“退出”动作，则关闭应用程序
    connect(exitAction, &QAction::triggered, this, &QApplication::quit);
}

MainWindow::~MainWindow()
{
    delete ui;
}
