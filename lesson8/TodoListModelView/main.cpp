#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QListView>
#include <QStringListModel>
#include <QSortFilterProxyModel> // 用于排序和过滤

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget window;
    window.setWindowTitle("Model/View To-Do List");

    // 1. 创建模型和视图
    QStringListModel *model = new QStringListModel(&window);
    QListView *view = new QListView(&window);
    view->setModel(model);

    // 2. 创建UI控件
    QLineEdit *taskInput = new QLineEdit();
    QPushButton *addButton = new QPushButton("添加");
    QPushButton *deleteButton = new QPushButton("删除选中");

    // 3. 布局
    QVBoxLayout *layout = new QVBoxLayout(&window);
    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputLayout->addWidget(taskInput);
    inputLayout->addWidget(addButton);
    layout->addLayout(inputLayout);
    layout->addWidget(view);
    layout->addWidget(deleteButton);

    // 4. 信号与槽
    QObject::connect(addButton, &QPushButton::clicked, addButton, [=]() {
        QString task = taskInput->text().trimmed();
        if (task.isEmpty()) return;
        // 直接操作模型，视图会自动更新！
        model->insertRow(model->rowCount());
        QModelIndex index = model->index(model->rowCount() - 1);
        model->setData(index, task);
        taskInput->clear();
    });

    QObject::connect(deleteButton, &QPushButton::clicked, deleteButton, [=]() {
        QModelIndexList selected = view->selectionModel()->selectedIndexes();
        // 从后往前删，避免索引错乱
        for (int i = selected.count() - 1; i >= 0; --i) {
            model->removeRow(selected[i].row());
        }
    });

    window.show();
    return a.exec();
}
