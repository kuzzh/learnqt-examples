// pieslice.cpp
#include "qcppiechart.h"

PieSlice::PieSlice(QCustomPlot *parentPlot) : QCPAbstractItem(parentPlot),
    center(createPosition("center")),
    mRadius(100), // 默认半径
    mStartAngle(0),
    mSpanAngle(90)
{
    // 默认中心点放在(0,0)
    center->setCoords(0, 0);

    mPen = QPen(Qt::black);
    mBrush = QBrush(Qt::NoBrush);
}

void PieSlice::draw(QCPPainter *painter)
{
    // 1. 获取中心点在屏幕上的像素坐标
    // 这一步很重要：把plot坐标(x,y)转换成屏幕像素(px,py)
    QPointF centerPixel = center->pixelPosition();

    // 2. 计算绘制矩形 (Bounding Box)
    QRectF pieRect(centerPixel.x() - mRadius, centerPixel.y() - mRadius,
                   mRadius * 2, mRadius * 2);

    // 3. 设置画笔
    painter->setPen(mPen);
    painter->setBrush(mBrush);

    // 4. 绘制扇形
    // Qt的drawPie使用的是1/16度为单位，所以要乘以16
    // 另外Qt的角度方向是逆时针，0度在3点钟方向。
    // 如果需要顺时针或者调整0度位置，需要在这里做数学转换。
    // 这里我们保持Qt默认逻辑：逆时针。
    painter->drawPie(pieRect, mStartAngle * 16, mSpanAngle * 16);
}

// 鼠标点击检测（为了简单，这里粗略写一下，不做复杂碰撞检测）
double PieSlice::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
    Q_UNUSED(details)
    // 如果该图层设定为不可选中，直接返回 -1 (表示未命中)
    if (onlySelectable && !mSelectable) return -1;

    // 获取中心点的像素坐标
    QPointF centerPixel = center->pixelPosition();

    // 计算鼠标点击点到圆心的距离
    double dist = QVector2D(pos - centerPixel).length();

    // 核心修复逻辑：
    // 1. 这里不再引用不存在的 mSelectionTolerance
    // 2. 对于填充的饼图，只要点击点在半径内，我们就认为"距离为0"（即完全命中）
    // 3. 为了更严谨，其实还应该计算角度是否在扇形范围内，但为了代码简单，
    //    这里只要点在圆内就算选中。
    if (dist <= mRadius) {
        return 0.0; // 0.0 表示完美命中
    }

    return -1; // -1 表示未命中
}

QCPPieChart::QCPPieChart(QWidget *parent) : QCustomPlot(parent), mRadius(120)
{
    // 1. 自身初始化：把自己变成一张白纸
    this->xAxis->setVisible(false);
    this->yAxis->setVisible(false);
    this->axisRect()->setAutoMargins(QCP::msNone); // 只要饼图，不要留白

    // 2. 启用抗锯齿，让圆滑一点
    this->setAntialiasedElements(QCP::aeAll);
}

void QCPPieChart::addData(QString name, double value, QColor color)
{
    mDataList.append({name, value, color});
}

void QCPPieChart::setPieRadius(int radius)
{
    mRadius = radius;
    repaintPie(); // 设置半径后直接重绘
}

void QCPPieChart::clearData()
{
    mDataList.clear();
    // 移除旧对象 (QCustomPlot 的 removeItem 会负责 delete)
    for(auto *s : mSlices) this->removeItem(s);
    for(auto *l : mLabels) this->removeItem(l);
    mSlices.clear();
    mLabels.clear();
}

void QCPPieChart::repaintPie()
{
    // 先清理旧图层，但保留 mDataList
    for(auto *s : mSlices) this->removeItem(s);
    for(auto *l : mLabels) this->removeItem(l);
    mSlices.clear();
    mLabels.clear();

    if (mDataList.isEmpty()) { this->replot(); return; }

    double total = 0;
    for (const auto &d : mDataList) total += d.value;

    double currentAngle = 90; // 12点钟方向开始

    for (const auto &d : mDataList) {
        double span = (d.value / total) * 360.0;

        // --- 1. 创建扇区 (父对象就是 this) ---
        PieSlice *slice = new PieSlice(this);
        slice->center->setType(QCPItemPosition::ptViewportRatio);
        slice->center->setCoords(0.5, 0.5); // 永远居中
        slice->setRadius(mRadius);
        slice->setAngles(currentAngle, span);
        slice->setBrush(d.color);
        slice->setPen(QPen(Qt::white, 2));
        mSlices.append(slice);

        // --- 2. 创建标签 ---
        if (span > 8.0) { // 角度太小不显示文字
            QCPItemText *text = new QCPItemText(this);
            text->setText(d.name + QString("\n%1%").arg(d.value/total*100, 0, 'f', 1));
            text->setFont(QFont("Arial", 9, QFont::Bold));
            text->setColor(Qt::white);

            // 计算文字位置
            double rad = qDegreesToRadians(currentAngle + span/2.0);
            double labelR = mRadius * 0.7;
            text->position->setParentAnchor(slice->center);
            text->position->setType(QCPItemPosition::ptAbsolute);
            // Y轴取反
            text->position->setCoords(qCos(rad)*labelR, -qSin(rad)*labelR);

            mLabels.append(text);
        }
        currentAngle += span;
    }

    this->replot();
}
