#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include <QPlainTextEdit>

namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindDialog(QWidget *parent = nullptr, QPlainTextEdit *editor = nullptr);
    ~FindDialog();

private slots:
    void on_findButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::FindDialog *ui;
    QPlainTextEdit *m_editor;   // 用于操作主编辑器
};

#endif // FINDDIALOG_H
