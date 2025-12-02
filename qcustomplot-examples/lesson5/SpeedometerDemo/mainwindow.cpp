// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtMath> // 记得包含这个，我们要用到Sin和Cos

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupSpeedometer();

    // 信号槽连接，滑块一动，指针就得动
    connect(ui->speedSlider, &QSlider::valueChanged, this, &MainWindow::onSpeedChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupSpeedometer()
{
    // --- 1. 搞定画布 ---
    // 加个标题
    ui->customPlot->plotLayout()->insertRow(0);
    ui->customPlot->plotLayout()->addElement(0, 0, new QCPTextElement(ui->customPlot, "汽车速度仪表盘", QFont("sans", 12, QFont::Bold)));

    // 既然是仪表盘，坐标轴就显得多余了，直接隐藏
    ui->customPlot->xAxis->setVisible(false);
    ui->customPlot->yAxis->setVisible(false);

    // 锁定坐标范围，防止拖拽缩放把仪表盘弄变形
    ui->customPlot->xAxis->setRange(-1.2, 1.2);
    ui->customPlot->yAxis->setRange(-1.0, 1.2);

    // --- 2. 苦力活：画刻度 ---
    // 设定：左下角(225度)是0，右下角(-45度)是240。总跨度270度。
    // 注意：QCP的角度计算和数学里一样，逆时针是正方向。

    double radius = 0.8; // 表盘半径
    for (int i = 0; i <= 240; i += 20) {
        // 核心数学公式：线性映射
        // 速度 0 -> 240  映射到  角度 225 -> -45
        double angleDeg = 225 - (i / 240.0) * 270.0;
        double angleRad = qDegreesToRadians(angleDeg);

        // 极坐标转直角坐标：x = r * cos(θ), y = r * sin(θ)
        double x1 = qCos(angleRad) * radius;
        double y1 = qSin(angleRad) * radius;
        double x2 = qCos(angleRad) * (radius - 0.1); // 刻度线往里缩一点
        double y2 = qSin(angleRad) * (radius - 0.1);

        // 画刻度线
        QCPItemLine *tick = new QCPItemLine(ui->customPlot);
        tick->start->setCoords(x1, y1);
        tick->end->setCoords(x2, y2);
        tick->setPen(QPen(Qt::black, 2));

        // 画文字标签（稍微再往里缩一点，免得和刻度重叠）
        double labelX = qCos(angleRad) * (radius - 0.25);
        double labelY = qSin(angleRad) * (radius - 0.25);
        QCPItemText *label = new QCPItemText(ui->customPlot);
        label->setText(QString::number(i));
        label->position->setCoords(labelX, labelY);
        label->setFont(QFont("Microsoft YaHei", 8)); // 换个好看点的字体
    }

    // --- 3. 初始化指针 ---
    mArrow = new QCPItemLine(ui->customPlot);
    mArrow->start->setCoords(0, 0); // 起点永远在圆心
    mArrow->end->setCoords(0, 0);   // 终点待会算
    mArrow->setHead(QCPLineEnding::esSpikeArrow); // 搞个尖头
    mArrow->setPen(QPen(Qt::red, 3));

    // --- 4. 装饰一下 ---
    // 指针根部通常有个黑圆盖住，这样好看点
    QCPItemEllipse *centerCircle = new QCPItemEllipse(ui->customPlot);
    centerCircle->topLeft->setCoords(-0.1, 0.1);
    centerCircle->bottomRight->setCoords(0.1, -0.1);
    centerCircle->setBrush(Qt::black);
    centerCircle->setPen(Qt::NoPen);

    // 归零
    onSpeedChanged(0);
}

void MainWindow::onSpeedChanged(int value)
{
    // 这里代码和上面画刻度其实是一样的逻辑
    double angleDeg = 225 - (value / 240.0) * 270.0;
    double angleRad = qDegreesToRadians(angleDeg);

    // 指针稍微短一点(0.7)，别戳出表盘了
    double x = qCos(angleRad) * 0.7;
    double y = qSin(angleRad) * 0.7;

    // 更新终点位置
    mArrow->end->setCoords(x, y);

    // 别忘了刷新，否则界面不会变
    ui->customPlot->replot();
}
