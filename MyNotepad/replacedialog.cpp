#include "replacedialog.h"
#include "ui_replacedialog.h"
#include <QTextCursor>
#include <QMessageBox>

ReplaceDialog::ReplaceDialog(QWidget *parent,
                             QPlainTextEdit *editor) :
    QDialog(parent),
    ui(new Ui::ReplaceDialog),
    m_editor(editor)
{
    ui->setupUi(this);

    ui->downwardBtn->setChecked(true); // 默认向下查找
}

ReplaceDialog::~ReplaceDialog()
{
    delete ui;
}

/*-----------------------------------------
 * 内部函数：查找一次
 ----------------------------------------*/
bool ReplaceDialog::findNext()
{
    QString target = ui->findEdit->text();
    if (target.isEmpty()) return false;

    QTextDocument::FindFlags flags;

    if (ui->matchCaseCheck->isChecked())
        flags |= QTextDocument::FindCaseSensitively;

    if (ui->upwardBtn->isChecked())
        flags |= QTextDocument::FindBackward;

    bool found = m_editor->find(target, flags);

    if (!found)
        QMessageBox::information(this, "提示", "找不到指定内容！");

    return found;
}

/*-----------------------------------------
 * 查找按钮
 ----------------------------------------*/
void ReplaceDialog::on_findButton_clicked()
{
    findNext();
}

/*-----------------------------------------
 * 替换按钮
 ----------------------------------------*/
void ReplaceDialog::on_replaceButton_clicked()
{
    if (!m_editor) return;

    QString target = ui->findEdit->text();
    QString replacement = ui->replaceEdit->text();
    if (target.isEmpty()) return;

    QTextDocument::FindFlags flags;

    if (ui->matchCaseCheck->isChecked())
        flags |= QTextDocument::FindCaseSensitively;

    if (ui->upwardBtn->isChecked())
        flags |= QTextDocument::FindBackward;

    // 先查找一次
    bool found = m_editor->find(target, flags);

    // 如果没找到 → 进行循环查找
    if (!found)
    {
        QTextCursor cursor = m_editor->textCursor();
        if (ui->upwardBtn->isChecked())
            cursor.movePosition(QTextCursor::End);
        else
            cursor.movePosition(QTextCursor::Start);

        m_editor->setTextCursor(cursor);

        found = m_editor->find(target, flags);
    }

    if (!found)
    {
        QMessageBox::information(this, "提示", "找不到指定内容！");
        return;
    }

    // 找到了 → 进行替换
    QTextCursor cursor = m_editor->textCursor();
    cursor.insertText(replacement);
}

/*-----------------------------------------
 * 全部替换
 ----------------------------------------*/
void ReplaceDialog::on_replaceAllButton_clicked()
{
    if (!m_editor) return;

    QString target = ui->findEdit->text();
    QString replacement = ui->replaceEdit->text();
    if (target.isEmpty()) return;

    QTextDocument::FindFlags flags;

    if (ui->matchCaseCheck->isChecked())
        flags |= QTextDocument::FindCaseSensitively;

    int count = 0;

    // 从头开始查找
    QTextCursor cursor = m_editor->textCursor();
    cursor.movePosition(QTextCursor::Start);
    m_editor->setTextCursor(cursor);

    // 循环查找并替换
    while (m_editor->find(target, flags))
    {
        QTextCursor c = m_editor->textCursor();
        c.insertText(replacement);
        count++;
    }

    QMessageBox::information(
        this,
        "替换完成",
        QString("已替换 %1 处内容").arg(count)
    );
}

/*-----------------------------------------
 * 取消按钮
 ----------------------------------------*/
void ReplaceDialog::on_cancelButton_clicked()
{
    close();
}
