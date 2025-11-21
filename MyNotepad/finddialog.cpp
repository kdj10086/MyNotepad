#include "finddialog.h"
#include "ui_finddialog.h"
#include <QMessageBox>

FindDialog::FindDialog(QWidget *parent, QPlainTextEdit *editor) :
    QDialog(parent),
    ui(new Ui::FindDialog),
    m_editor(editor)
{
    ui->setupUi(this);

    ui->downwardBtn->setChecked(true);  // 默认向下搜索
}

FindDialog::~FindDialog()
{
    delete ui;
}

void FindDialog::on_findButton_clicked()
{
    if (!m_editor) return;

    QString target = ui->findEdit->text();
    if (target.isEmpty()) return;

    QTextDocument::FindFlags flags;

    if (ui->matchCaseCheck->isChecked())
        flags |= QTextDocument::FindCaseSensitively;

    if (ui->upwardBtn->isChecked())
        flags |= QTextDocument::FindBackward;

    // 第一次查找
    bool found = m_editor->find(target, flags);

    // 如果没找到 → 从头/从尾 重试一次
    if (!found)
    {
        // 重置光标到头或尾
        QTextCursor cursor = m_editor->textCursor();
        if (ui->upwardBtn->isChecked())
            cursor.movePosition(QTextCursor::End);
        else
            cursor.movePosition(QTextCursor::Start);

        m_editor->setTextCursor(cursor);

        // 再查找一次
        found = m_editor->find(target, flags);
    }

    if (!found)
        QMessageBox::information(this, "提示", "找不到指定内容！");
}

void FindDialog::on_cancelButton_clicked()
{
    close();
}
