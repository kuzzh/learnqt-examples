// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVector>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("年度销售额统计图");
    setupSalesBarChart(); // 调用柱状图设置函数
}

void MainWindow::setupSalesBarChart()
{
    // --- 1. 准备数据 ---
    // 模拟12个月的销售额数据
    QVector<double> salesData;
    salesData << 120 << 150 << 130 << 180 << 200 << 220
              << 250 << 230 << 210 << 190 << 160 << 140; // 单位：万元

    // X轴的刻度位置 (1到12)
    QVector<double> ticks;
    for (int i = 1; i <= 12; ++i) {
        ticks << i;
    }

    // X轴的刻度标签 (月份名称)
    QVector<QString> labels;
    labels << "一月" << "二月" << "三月" << "四月" << "五月" << "六月"
           << "七月" << "八月" << "九月" << "十月" << "十一月" << "十二月";

    // --- 2. 创建 QCPBars 对象 ---
    QCPBars *salesBars = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);

    // --- 3. 设置柱状图样式 ---
    salesBars->setName("年度销售额"); // 设置图例名称
    salesBars->setWidth(0.6); // 设置柱子的宽度，相对于刻度间隔
    salesBars->setData(ticks, salesData); // 传入数据

    // 设置柱子的颜色和边框
    QPen pen;
    pen.setWidthF(1.2);
    salesBars->setPen(pen);
    salesBars->setBrush(QColor(100, 180, 255, 150)); // 蓝色，半透明

    // --- 4. 设置X轴 (分类轴) ---
    // 使用QCPAxisTickerText来显示自定义的文本标签
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels); // 将刻度位置和标签关联起来
    ui->customPlot->xAxis->setTicker(textTicker); // 应用到X轴
    ui->customPlot->xAxis->setTickLabelRotation(60); // 标签旋转60度，避免重叠
    ui->customPlot->xAxis->setSubTicks(false); // 不显示子刻度
    ui->customPlot->xAxis->setTickLength(0, 4); // 刻度线长度
    ui->customPlot->xAxis->setRange(0.5, 12.5); // 设置X轴范围，确保所有柱子可见
    ui->customPlot->xAxis->setLabel("月份");

    // --- 5. 设置Y轴 ---
    ui->customPlot->yAxis->setLabel("销售额 (万元)");
    ui->customPlot->yAxis->setRange(0, 300); // 设置Y轴范围
    ui->customPlot->yAxis->setPadding(5); // 顶部留白

    // --- 6. 添加图表标题和图例 ---
    ui->customPlot->plotLayout()->insertRow(0);
    ui->customPlot->plotLayout()->addElement(0, 0, new QCPTextElement(ui->customPlot, "2023年度销售额统计", QFont("sans", 17, QFont::Bold)));
    ui->customPlot->legend->setVisible(true); // 显示图例
    ui->customPlot->legend->setBrush(QColor(255, 255, 255, 150));

    // --- 7. 启用交互功能 (可选) ---
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->customPlot->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    ui->customPlot->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);

    // --- 8. 重绘图表 ---
    ui->customPlot->replot();
}

MainWindow::~MainWindow()
{
    delete ui;
}
