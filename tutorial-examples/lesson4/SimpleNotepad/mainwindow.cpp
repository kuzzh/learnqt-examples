#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QTextEdit>
#include <QFileDialog> // 文件对话框
#include <QFile>       // 文件操作
#include <QTextStream> // 文本流
#include <QMessageBox> // 消息框

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 将textEdit的创建和设置移到这里
    textEdit = new QTextEdit(this);
    setCentralWidget(textEdit);

    // --- 创建Actions并添加图标 ---
    // 注意路径前缀 ":/"，它指向我们的qrc资源
    QAction *newAction = new QAction(QIcon(":/icons/file-plus.svg"), "新建", this);
    QAction *openAction = new QAction(QIcon(":/icons/folder.svg"), "打开", this);
    QAction *saveAction = new QAction(QIcon(":/icons/save.svg"), "保存", this); // 新增保存Action
    QAction *exitAction = new QAction(QIcon(":/icons/x.svg"), "退出", this);

    exitAction->setShortcut(QKeySequence("Ctrl+Q"));

    // --- 菜单栏 ---
    QMenu *fileMenu = menuBar()->addMenu("文件(&F)");
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction); // 添加到菜单
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    // --- 工具栏 ---
    QToolBar *fileToolBar = addToolBar("文件");
    fileToolBar->addAction(newAction);
    fileToolBar->addAction(openAction);
    fileToolBar->addAction(saveAction); // 添加到工具栏

    statusBar()->showMessage("准备就绪...");

    // --- 连接信号与槽 ---
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);
    connect(exitAction, &QAction::triggered, this, &QApplication::quit);
    // "新建" 暂时只清空文本
    connect(newAction, &QAction::triggered, textEdit, &QTextEdit::clear);
}

// --- 实现槽函数 ---

void MainWindow::openFile()
{
    // 弹出文件对话框，让用户选择一个文件
    QString filePath = QFileDialog::getOpenFileName(this, "打开文件", "", "文本文件 (*.txt);;所有文件 (*.*)");
    if (filePath.isEmpty()) { // 如果用户取消了选择
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "警告", "无法打开文件: " + file.errorString());
        return;
    }

    QTextStream in(&file);
    in.setCodec("utf-8");
    QString content = in.readAll();
    textEdit->setText(content);
    file.close();

    statusBar()->showMessage("文件已打开: " + filePath, 2000);
}

void MainWindow::saveFile()
{
    QString filePath = QFileDialog::getSaveFileName(this, "保存文件", "", "文本文件 (*.txt);;所有文件 (*.*)");
    if (filePath.isEmpty()) {
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "警告", "无法保存文件: " + file.errorString());
        return;
    }

    QTextStream out(&file);
    QString content = textEdit->toPlainText(); // 获取纯文本内容
    out << content;
    file.close();

    statusBar()->showMessage("文件已保存: " + filePath, 2000);
}

MainWindow::~MainWindow()
{
    delete ui;
}
