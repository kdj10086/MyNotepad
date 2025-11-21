#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QString m_filePath;      // 当前文件路径，为空表示“新建文档”
    bool    m_isTextChanged; // 是否被修改

    bool maybeSave();                        // 询问是否保存
    bool saveToFile(const QString &path);    // 真正的保存实现
    bool loadFromFile(const QString &path);  // 真正的打开实现
    void updateStatusBar();
    void updateWindowTitle();                // 根据状态更新标题栏

    QLabel statusCursorLabel;
    QLabel statusLabel;

protected:
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void on_actionNew_triggered();      // 文件-新建
    void on_actionOpen_triggered();     // 文件-打开
    void on_actionSave_triggered();     // 文件-保存
    void on_actionSaveAs_triggered();   // 文件-另存为

    void onEditorTextChanged();         // 文本改变时的槽

    void on_actionAbout_triggered();
    void on_actionFind_triggered();
    void on_actionReplace_triggered();
    void on_actionCopy_triggered();
    void on_actionCut_triggered();
    void on_actionPaste_triggered();
    void on_actionFont_triggered();
    void on_actionBackgroundColor_triggered();
    void on_actionWrap_triggered(bool checked);
    void on_actionViewToolbar_triggered(bool checked);
    void on_actionViewStatusbar_triggered(bool checked);
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
};
#endif // MAINWINDOW_H
