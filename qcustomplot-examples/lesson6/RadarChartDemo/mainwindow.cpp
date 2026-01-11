#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtMath>
#include <QVector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupRadarChart();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupRadarChart()
{
    // --- 1. 基础设置 ---
    // 添加标题
    ui->customPlot->plotLayout()->insertRow(0);
    ui->customPlot->plotLayout()->addElement(0, 0, new QCPTextElement(ui->customPlot, "角色能力分布图", QFont("sans", 14, QFont::Bold)));

    // 隐藏默认坐标轴
    ui->customPlot->xAxis->setVisible(false);
    ui->customPlot->yAxis->setVisible(false);

    // 设置坐标范围 (留出边距)
    ui->customPlot->xAxis->setRange(-1.5, 1.5);
    ui->customPlot->yAxis->setRange(-1.5, 1.5);

    // --- 2. 定义数据和维度 ---
    int dimensionCount = 5;
    QVector<QString> labels;
    labels << "攻击" << "防御" << "魔法" << "敏捷" << "体力";

    QVector<double> values;
    values << 0.9 << 0.6 << 0.8 << 0.7 << 0.5;

    // --- 3. 绘制背景网格 ---
    for (int level = 1; level <= 4; ++level) {
        double radius = level * 0.25;

        // 创建曲线对象，绑定到 x, y 轴
        QCPCurve *gridCurve = new QCPCurve(ui->customPlot->xAxis, ui->customPlot->yAxis);
        gridCurve->setPen(QPen(QColor(200, 200, 200), 1, Qt::DashLine)); // 灰色虚线
        gridCurve->setBrush(Qt::NoBrush); // 无填充
        // 不在图例中显示网格线
        gridCurve->removeFromLegend();

        QVector<double> xPoints, yPoints;
        // 计算5个顶点
        for (int i = 0; i < dimensionCount; ++i) {
            double angleDeg = 90.0 + i * (360.0 / dimensionCount);
            double angleRad = qDegreesToRadians(angleDeg);

            xPoints << radius * qCos(angleRad);
            yPoints << radius * qSin(angleRad);
        }
        // 为了形成闭合多边形，必须把第一个点再加到末尾
        xPoints << xPoints[0];
        yPoints << yPoints[0];

        // 设置数据
        gridCurve->setData(xPoints, yPoints);
    }

    // --- 4. 绘制放射轴线 ---
    for (int i = 0; i < dimensionCount; ++i) {
        double angleDeg = 90.0 + i * (360.0 / dimensionCount);
        double angleRad = qDegreesToRadians(angleDeg);

        double x = 1.0 * qCos(angleRad);
        double y = 1.0 * qSin(angleRad);

        QCPItemLine *axisLine = new QCPItemLine(ui->customPlot);
        axisLine->start->setCoords(0, 0);
        axisLine->end->setCoords(x, y);
        axisLine->setPen(QPen(QColor(180, 180, 180), 1));

        // 绘制标签
        double labelX = 1.15 * qCos(angleRad);
        double labelY = 1.15 * qSin(angleRad);

        QCPItemText *label = new QCPItemText(ui->customPlot);
        label->setText(labels[i]);
        label->position->setCoords(labelX, labelY);
        label->setFont(QFont("Microsoft YaHei", 10));
        label->setPositionAlignment(Qt::AlignCenter);
    }

    // --- 5. 绘制数据区域 ---
    QCPCurve *dataCurve = new QCPCurve(ui->customPlot->xAxis, ui->customPlot->yAxis);
    dataCurve->setPen(QPen(QColor(0, 168, 255), 2)); // 蓝色边框
    dataCurve->setBrush(QColor(0, 168, 255, 100));   // 蓝色半透明填充
    dataCurve->setName("英雄A");

    QVector<double> dataX, dataY;
    for (int i = 0; i < dimensionCount; ++i) {
        double angleDeg = 90.0 + i * (360.0 / dimensionCount);
        double angleRad = qDegreesToRadians(angleDeg);

        double radius = values[i];
        dataX << radius * qCos(angleRad);
        dataY << radius * qSin(angleRad);
    }
    // 闭合多边形
    dataX << dataX[0];
    dataY << dataY[0];

    dataCurve->setData(dataX, dataY);

    // 开启图例显示
    ui->customPlot->legend->setVisible(true);

    // 设置图例的字体
    ui->customPlot->legend->setFont(QFont("Microsoft YaHei", 9));

    ui->customPlot->replot();
}
