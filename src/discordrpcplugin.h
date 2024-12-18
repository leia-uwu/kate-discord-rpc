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

    int configPages() const override
    {
        return 1;
    }
    KTextEditor::ConfigPage *configPage(int number = 0, QWidget *parent = nullptr) override;

    struct RPCConfig {
        QString detailsText;
        QString stateText;
        bool showElapsedTime;
    };

    static RPCConfig DefaultConfig;

    RPCConfig *m_config = new RPCConfig();
    void readConfig();
    void updateStatus();

private:
    KTextEditor::MainWindow *m_mainWindow;

    int64_t m_startTimestamp;

    void initDiscord();
};

#endif
