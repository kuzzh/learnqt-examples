#include "paintwidget.h"
#include <QPainter>
#include <QMouseEvent>

PaintWidget::PaintWidget(QWidget *parent) : QWidget(parent) { }

void PaintWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this); // 创建一个QPainter对象，指定“画板”为本窗口
    painter.setRenderHint(QPainter::Antialiasing); // 设置抗锯齿，让线条更平滑

    // 设置画笔颜色和宽度
    QPen pen(Qt::cyan);
    pen.setWidth(3);
    painter.setPen(pen);

    // 遍历并绘制所有已经记录的线段
    for (const QLine &line : lines) {
        painter.drawLine(line);
    }
}

void PaintWidget::mousePressEvent(QMouseEvent *event)
{
    // 鼠标按下时，记录当前点
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
    }
}

void PaintWidget::mouseMoveEvent(QMouseEvent *event)
{
    // 鼠标左键按住并移动时，才进行绘制
    if (event->buttons() & Qt::LeftButton) {
        // 创建一条从上一个点到当前点的线段，并添加到列表中
        lines.append(QLine(lastPoint, event->pos()));
        lastPoint = event->pos(); // 更新上一个点的位置
        update(); // 触发重绘事件 (paintEvent)
    }
}
