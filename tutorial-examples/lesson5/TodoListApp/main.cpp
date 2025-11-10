#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>  // 网格布局
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>  // 列表控件
#include <QCheckBox>    // 复选框

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget window;
    window.setWindowTitle("我的待办事项");

    // --- 创建所有控件 ---
    QLineEdit *taskInput = new QLineEdit;
    taskInput->setPlaceholderText("输入新任务..."); // 设置提示文字

    QPushButton *addButton = new QPushButton("添加");
    QListWidget *taskList = new QListWidget;
    QPushButton *deleteButton = new QPushButton("删除已完成");

    // --- 使用网格布局 QGridLayout ---
    QGridLayout *layout = new QGridLayout;
    // addWidget(控件, 行, 列, 占用行数, 占用列数)
    layout->addWidget(taskInput, 0, 0);            // 第0行，第0列
    layout->addWidget(addButton, 0, 1);            // 第0行，第1列
    layout->addWidget(taskList, 1, 0, 1, 2);       // 第1行，第0列，占1行，占2列
    layout->addWidget(deleteButton, 2, 0, 1, 2);   // 第2行，第0列，占1行，占2列

    window.setLayout(layout);

    // --- 信号与槽连接 ---

    // 1. 添加任务
    QObject::connect(addButton, &QPushButton::clicked, [&]() {
        QString taskText = taskInput->text().trimmed(); // .trimmed()移除前后空格
        if (taskText.isEmpty()) {
            return; // 不添加空任务
        }

        // 创建一个 QListWidgetItem (列表项)
        QListWidgetItem *newItem = new QListWidgetItem();
        // 创建一个 QCheckBox (复选框) 作为任务项的实际显示内容
        QCheckBox *taskCheckbox = new QCheckBox(taskText);

        // 将列表项添加到列表中
        taskList->addItem(newItem);
        // **关键：将复选框设置为这个列表项的“控件”**
        taskList->setItemWidget(newItem, taskCheckbox);

        taskInput->clear(); // 清空输入框
        taskInput->setFocus(); // 让光标回到输入框
    });

    // 2. 删除已完成的任务
    QObject::connect(deleteButton, &QPushButton::clicked, [&]() {
        // **重要：从后往前遍历并删除，避免因索引变化导致崩溃**
        for (int i = taskList->count() - 1; i >= 0; --i) {
            QListWidgetItem *item = taskList->item(i);
            // 获取该项对应的控件，并转换为QCheckBox指针
            QCheckBox *checkbox = qobject_cast<QCheckBox*>(taskList->itemWidget(item));

            if (checkbox && checkbox->isChecked()) {
                // 如果复选框被选中，则删除该项
                delete taskList->takeItem(i); // takeItem会移除项并返回指针，需要手动delete
            }
        }
    });

    window.resize(400, 350);
    window.show();

    return a.exec();
}
