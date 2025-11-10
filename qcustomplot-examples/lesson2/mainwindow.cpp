// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime> // 用于处理时间数据

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("模拟股票趋势图");
    setupStockPlot(); // 调用新的绘图函数
}

void MainWindow::setupStockPlot()
{
    // --- 1. 设置图表标题 ---
    ui->customPlot->plotLayout()->insertRow(0); // 在顶部插入一行
    ui->customPlot->plotLayout()->addElement(0, 0, new QCPTextElement(ui->customPlot, "模拟股票趋势图", QFont("sans", 17, QFont::Bold)));

    // --- 2. 准备模拟股票数据 ---
    // x轴为时间，y轴为价格
    QVector<double> timeData, priceData;
    QDateTime startTime = QDateTime::currentDateTime().addDays(-30); // 从30天前开始
    qsrand(QDateTime::currentMSecsSinceEpoch()); // 初始化随机数种子

    double currentPrice = 100.0;
    for (int i = 0; i < 30; ++i) {
        timeData.append(startTime.addDays(i).toTime_t()); // 将日期转换为Unix时间戳
        currentPrice += (qrand() % 20 - 10) / 10.0; // 价格随机波动
        priceData.append(currentPrice);
    }

    // --- 3. 添加图层 (QCPGraph) ---
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(timeData, priceData);
    ui->customPlot->graph(0)->setName("股票价格"); // 设置图层名称，用于图例

    // --- 4. 定制曲线样式 ---
    QPen graphPen;
    graphPen.setColor(QColor(255, 100, 0)); // 橙色
    graphPen.setWidth(2); // 2像素宽
    ui->customPlot->graph(0)->setPen(graphPen);
    ui->customPlot->graph(0)->setLineStyle(QCPGraph::lsLine); // 实线
    ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5)); // 数据点显示为圆形，大小5

    // --- 5. 设置坐标轴 ---
    // x轴显示时间
    QSharedPointer<QCPAxisTickerDateTime> dateTimeTicker(new QCPAxisTickerDateTime);
    dateTimeTicker->setDateTimeFormat("yyyy-MM-dd"); // 设置日期格式
    ui->customPlot->xAxis->setTicker(dateTimeTicker);
    ui->customPlot->xAxis->setLabel("日期");
    ui->customPlot->yAxis->setLabel("价格");

    // 自动调整坐标轴范围以适应数据
    ui->customPlot->graph(0)->rescaleAxes();
    // 稍微留出一些边距，让图表看起来不那么拥挤
    ui->customPlot->xAxis->setRange(ui->customPlot->xAxis->range().lower - 86400 * 0.5, ui->customPlot->xAxis->range().upper + 86400 * 0.5); // 0.5天
    ui->customPlot->yAxis->setRange(ui->customPlot->yAxis->range().lower * 0.9, ui->customPlot->yAxis->range().upper * 1.1);

    // --- 6. 添加图例 ---
    ui->customPlot->legend->setVisible(true); // 显示图例
    ui->customPlot->legend->setFont(QFont("Helvetica", 9)); // 设置图例字体
    ui->customPlot->legend->setBrush(QBrush(QColor(255, 255, 255, 150))); // 半透明背景

    // --- 7. 重绘图表 ---
    // ui->customPlot->replot();

    // --- 8. 启用交互功能 ---
    // 允许用户通过鼠标拖动来平移坐标轴
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    // 允许用户通过鼠标滚轮来缩放坐标轴
    // QCP::iRangeZoomX 仅缩放X轴，QCP::iRangeZoomY 仅缩放Y轴，QCP::iRangeZoom 同时缩放X和Y轴
    ui->customPlot->axisRect()->setRangeDrag(Qt::Horizontal); // 仅允许水平拖动
    ui->customPlot->axisRect()->setRangeZoom(Qt::Horizontal); // 仅允许水平缩放

    // --- 9. 响应鼠标点击事件 (可选，用于数据点选择) ---
    connect(ui->customPlot, &QCustomPlot::mousePress, this, [this](QMouseEvent *event){
        // 示例：点击图表时打印鼠标位置
        qDebug() << "Mouse clicked at:" << event->pos();
    });

    // --- 10. 重绘图表 ---
    ui->customPlot->replot();
}

MainWindow::~MainWindow()
{
    delete ui;
}
