#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include <QDialog>
#include <QPlainTextEdit>

namespace Ui {
class ReplaceDialog;
}

class ReplaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReplaceDialog(QWidget *parent,
                           QPlainTextEdit *editor);
    ~ReplaceDialog();

private slots:
    void on_findButton_clicked();
    void on_replaceButton_clicked();
    void on_replaceAllButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::ReplaceDialog *ui;
    QPlainTextEdit *m_editor;

    bool findNext();  // 内部函数
};

#endif // REPLACEDIALOG_H
