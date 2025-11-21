// pieslice.h
// 这是一个自定义的QCustomPlot图层项
#ifndef QCPPIECHART_H
#define QCPPIECHART_H

#include "qcustomplot.h"

class PieSlice : public QCPAbstractItem
{
    Q_OBJECT
public:
    PieSlice(QCustomPlot *parentPlot);

    // 定义两个位置：中心点，和圆周上的某点（用于决定半径）
    // 其实半径直接用数值指定更简单，但为了符合QCP习惯，我们还是保留Position
    QCPItemPosition *center;

    // 属性接口
    void setRadius(double pixelRadius) { mRadius = pixelRadius; }
    void setAngles(double startAngle, double spanAngle) { mStartAngle = startAngle; mSpanAngle = spanAngle; }
    void setPen(const QPen &pen) { mPen = pen; }
    void setBrush(const QBrush &brush) { mBrush = brush; }

    // QCP必须实现的虚函数
    virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const override;

protected:
    // 核心绘制函数
    virtual void draw(QCPPainter *painter) override;

    double mRadius;
    double mStartAngle; // 角度制，0-360
    double mSpanAngle;
    QPen mPen;
    QBrush mBrush;
};

class QCPPieChart : public QCustomPlot
{
    Q_OBJECT
public:
    explicit QCPPieChart(QWidget *parent = nullptr);

    // 对外接口极其简单
    void addData(QString name, double value, QColor color);
    void setPieRadius(int radius); // 改个名避免和父类冲突
    void clearData();
    void repaintPie(); // 计算并重绘

private:
    struct SliceData { QString name; double value; QColor color; };
    QVector<SliceData> mDataList;
    QVector<PieSlice*> mSlices;      // 管理生成的扇区
    QVector<QCPItemText*> mLabels;   // 管理生成的文字
    int mRadius;
};

#endif // QCPPIECHART_H
