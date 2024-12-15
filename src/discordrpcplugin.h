#ifndef DISCORDRPCPLUGIN_H
#define DISCORDRPCPLUGIN_H

#include <KTextEditor/Plugin>
#include <ktexteditor/mainwindow.h>

class DiscordRpcPlugin : public KTextEditor::Plugin
{
    Q_OBJECT

public:
    explicit DiscordRpcPlugin(QObject *parent = nullptr, const QList<QVariant> & = QList<QVariant>());
    ~DiscordRpcPlugin() override;

    QObject *createView(KTextEditor::MainWindow *mainWindow) override;

private:
    KTextEditor::MainWindow *m_mainWindow;

    int64_t m_startTimestamp;

    void initDiscord();

    void updateStatus();
};

#endif
