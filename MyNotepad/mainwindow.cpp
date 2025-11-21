#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "finddialog.h"
#include "replacedialog.h"
#include "qfontdialog.h"
#include "qcolordialog.h"
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QFileInfo>
#include <QClipboard>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_isTextChanged(false)
{
    ui->setupUi(this);

    m_filePath.clear();
    updateWindowTitle();

    ui->actionUndo->setEnabled(false);
    ui->actionRedo->setEnabled(false);

    // 当有文本被选中时，启用剪切和复制按钮
    connect(ui->mainEditor, &QPlainTextEdit::selectionChanged, this, [this]() {
        bool hasSelection = !ui->mainEditor->textCursor().selectedText().isEmpty();
        ui->actionCut->setEnabled(hasSelection);
        ui->actionCopy->setEnabled(hasSelection);
    });

    // 当剪贴板有内容时，启用粘贴按钮
    QClipboard* clipboard = QApplication::clipboard();
    connect(clipboard, &QClipboard::dataChanged, this, [this]() {
        ui->actionPaste->setEnabled(!QApplication::clipboard()->text().isEmpty());
    });


    // 连接文本改变信号
    connect(ui->mainEditor, &QPlainTextEdit::textChanged,
            this, &MainWindow::onEditorTextChanged);

    // 当文本内容改变 → 更新 length / lines
    connect(ui->mainEditor, &QPlainTextEdit::textChanged,
            this, &MainWindow::updateStatusBar);

    // 当光标位置改变 → 更新 Ln / Col
    connect(ui->mainEditor, &QPlainTextEdit::cursorPositionChanged,
            this, &MainWindow::updateStatusBar);

    connect(ui->mainEditor, &QPlainTextEdit::undoAvailable,
            ui->actionUndo, &QAction::setEnabled);

    connect(ui->mainEditor, &QPlainTextEdit::redoAvailable,
            ui->actionRedo, &QAction::setEnabled);

    statusLabel.setMaximumWidth(250);
    statusLabel.setText("length: "+QString::number(0)+"    lines:   "+QString::number(1));
    ui->statusBar->addPermanentWidget(&statusLabel);

    statusCursorLabel.setMaximumWidth(250);
    statusCursorLabel.setText("Ln: "+QString::number(0)+"    Col: "+QString::number(1));
    ui->statusBar->addPermanentWidget(&statusCursorLabel);

    QLabel *author = new QLabel(ui->statusBar);
    author->setText(tr("刘业飞"));
    ui->statusBar->addPermanentWidget(author);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    QToolBar* tb = this->findChild<QToolBar*>("mainToolBar");
    if (!tb) return;

    int W = this->width();

    int iconSize = qMax(16,int( W * 0.03));

    tb->setIconSize(QSize(iconSize, iconSize));
}

void MainWindow::updateWindowTitle()
{
    QString name = m_filePath.isEmpty()
                   ? "新建文本文档"
                   : QFileInfo(m_filePath).fileName();

    if (m_isTextChanged)
        name.prepend("*");

    setWindowTitle(name + " - 记事本");
}

void MainWindow::onEditorTextChanged()
{
    m_isTextChanged = true;
    updateWindowTitle();
}

bool MainWindow::maybeSave()
{
    if (!m_isTextChanged)
        return true;   // 没改过，直接继续

    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this,
                               "记事本",
                               "文件已被修改，是否保存？",
                               QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

    if (ret == QMessageBox::Yes) {
        // 选择保存
        return saveToFile(m_filePath.isEmpty()
                          ? QFileDialog::getSaveFileName(this, "另存为", "",
                                                         "文本文件 (*.txt);;所有文件 (*.*)")
                          : m_filePath);
    } else if (ret == QMessageBox::No) {
        // 不保存
        return true;
    } else { // Cancel
        return false;
    }
}

bool MainWindow::loadFromFile(const QString &path)
{
    if (path.isEmpty())
        return false;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "打开失败",
                             "无法打开文件：\n" + path);
        return false;
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");       // 按需要设置编码
    QString text = in.readAll();
    file.close();

    ui->mainEditor->setPlainText(text);
    m_filePath = path;
    m_isTextChanged = false;
    updateWindowTitle();
    return true;
}

bool MainWindow::saveToFile(const QString &path)
{
    if (path.isEmpty())
        return false;

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "保存失败",
                             "无法保存文件：\n" + path);
        return false;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << ui->mainEditor->toPlainText();
    file.close();

    m_filePath = path;
    m_isTextChanged = false;
    updateWindowTitle();
    return true;
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog dlg;
    dlg.exec();
}


void MainWindow::on_actionFind_triggered()
{
    FindDialog dlg(this, ui->mainEditor);
    dlg.exec();
}

void MainWindow::on_actionReplace_triggered()
{
    ReplaceDialog dlg(this, ui->mainEditor);
    dlg.exec();
}

void MainWindow::on_actionNew_triggered()
{
    if (!maybeSave())
        return; // 用户点了取消

    ui->mainEditor->clear();
    m_filePath.clear();
    m_isTextChanged = false;
    updateWindowTitle();
}

void MainWindow::on_actionOpen_triggered()
{
    if (!maybeSave())
        return;

    QString path = QFileDialog::getOpenFileName(
                       this,
                       "打开文件",
                       "",
                       "文本文件 (*.txt);;所有文件 (*.*)");

    if (!path.isEmpty()) {
        loadFromFile(path);
    }
}

void MainWindow::on_actionSave_triggered()
{
    if (m_filePath.isEmpty()) {
        // 没有文件名，相当于“另存为”
        on_actionSaveAs_triggered();
    } else {
        saveToFile(m_filePath);
    }
}

void MainWindow::on_actionSaveAs_triggered()
{
    QString path = QFileDialog::getSaveFileName(
                       this,
                       "另存为",
                       m_filePath.isEmpty() ? "" : m_filePath,
                       "文本文件 (*.txt);;所有文件 (*.*)");

    if (!path.isEmpty()) {
        saveToFile(path);
    }
}

void MainWindow::on_actionCopy_triggered()
{
    ui->mainEditor->copy();
}


void MainWindow::on_actionCut_triggered()
{
    ui->mainEditor->cut();
}


void MainWindow::on_actionPaste_triggered()
{
    ui->mainEditor->paste();
}


void MainWindow::on_actionFont_triggered()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, ui->mainEditor->font(), this,
                                          tr("选择字体"));

    if (ok) {
        ui->mainEditor->setFont(font);
    }
}

void MainWindow::on_actionBackgroundColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::white, this, tr("选择背景颜色"));

    if (color.isValid()) {
        QPalette p = ui->mainEditor->palette();
        p.setColor(QPalette::Base, color);     // 编辑器背景色
        ui->mainEditor->setPalette(p);
    }
}

void MainWindow::on_actionWrap_triggered(bool checked)
{
    if (checked)
        ui->mainEditor->setLineWrapMode(QPlainTextEdit::WidgetWidth);  // 自动换行
    else
        ui->mainEditor->setLineWrapMode(QPlainTextEdit::NoWrap);       // 取消换行
}

void MainWindow::on_actionViewToolbar_triggered(bool checked)
{
    ui->mainToolBar->setVisible(checked);
}

void MainWindow::on_actionViewStatusbar_triggered(bool checked)
{
    ui->statusBar->setVisible(checked);
}

void MainWindow::updateStatusBar()
{
    // 文本内容
    QString text = ui->mainEditor->toPlainText();
    int length = text.length();

    int lines = ui->mainEditor->document()->blockCount();

    // 光标位置
    QTextCursor cursor = ui->mainEditor->textCursor();
    int line = cursor.blockNumber() + 1;   // blockNumber 从 0 开始
    int column = cursor.columnNumber() + 1;

    // 更新状态栏显示
    statusLabel.setText(
        QString("length: %1    lines: %2").arg(length).arg(lines)
    );

    statusCursorLabel.setText(
        QString("Ln: %1    Col: %2").arg(line).arg(column)
    );
}



void MainWindow::on_actionUndo_triggered()
{
    ui->mainEditor->undo();
}


void MainWindow::on_actionRedo_triggered()
{
    ui->mainEditor->redo();
}

