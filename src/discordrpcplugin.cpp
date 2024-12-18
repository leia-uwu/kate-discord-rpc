#include "discordrpcplugin.h"
#include "discordrpcconfigpage.h"

#include <QDebug>

#include <KConfigGroup>
#include <KPluginFactory>
#include <KSharedConfig>
#include <KTextEditor/Document>
#include <KTextEditor/MainWindow>
#include <KTextEditor/Plugin>
#include <KTextEditor/View>

#include <discord_rpc.h>

extern "C" {

static void handleReady(const DiscordUser *)
{
}

static void handleDisconnect(int, const char *)
{
}

static void handleError(int errorCode, const char *message)
{
    qWarning() << "DISCORD RPC ERROR: CODE: " << errorCode << "MESSAGE: " << message;
}

static void handleJoin(const char *)
{
}

static void handleSpectate(const char *)
{
}

static void handleJoinRequest(const DiscordUser *)
{
}

} // extern C

K_PLUGIN_FACTORY_WITH_JSON(DiscordRpcPluginFactory, "discordrpcplugin.json", registerPlugin<DiscordRpcPlugin>();)

DiscordRpcPlugin::DiscordRpcPlugin(QObject *parent, const QList<QVariant> &)
    : KTextEditor::Plugin(parent)
{
    readConfig();
}

DiscordRpcPlugin::RPCConfig DiscordRpcPlugin::DefaultConfig{
    "Editing {FILENAME}",
    "Project: {PROJECT}",
    true,
};

DiscordRpcPlugin::~DiscordRpcPlugin()
{
    Discord_Shutdown();
}

QObject *DiscordRpcPlugin::createView(KTextEditor::MainWindow *mainWindow)
{
    m_mainWindow = mainWindow;
    initDiscord();
    return new QObject(this);
}

KTextEditor::ConfigPage *DiscordRpcPlugin::configPage(int number, QWidget *parent)
{
    if (number == 0) {
        return new DiscordRpcConfigPage(parent, this);
    }
    return nullptr;
}

void DiscordRpcPlugin::readConfig()
{
    KConfigGroup config(KSharedConfig::openConfig(), QStringLiteral("DiscordRPC"));

    auto defaults = DiscordRpcPlugin::DefaultConfig;

    m_config->detailsText = config.readEntry("DetailsText", defaults.detailsText);
    m_config->stateText = config.readEntry("StateText", defaults.stateText);
    m_config->showElapsedTime = config.readEntry("ShowElapsedTime", defaults.showElapsedTime);
}

void DiscordRpcPlugin::initDiscord()
{
    DiscordEventHandlers handlers;
    memset(&handlers, 0, sizeof(handlers));
    handlers.ready = handleReady;
    handlers.disconnected = handleDisconnect;
    handlers.errored = handleError;
    handlers.joinGame = handleJoin;
    handlers.spectateGame = handleSpectate;
    handlers.joinRequest = handleJoinRequest;

    Discord_Initialize(DISCORD_ID, &handlers, 1, nullptr);

    m_startTimestamp = QDateTime::currentSecsSinceEpoch();

    updateStatus();
    connect(m_mainWindow, &KTextEditor::MainWindow::viewChanged, this, &DiscordRpcPlugin::updateStatus);
}

void DiscordRpcPlugin::updateStatus()
{
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));

    discordPresence.startTimestamp = m_config->showElapsedTime ? m_startTimestamp : 0;

    QString fileName = "";
    KTextEditor::View *view = m_mainWindow->activeView();
    if (view) {
        fileName = view->document()->url().fileName();
    }

    QString projectName = "";
    QObject *projectPlugin = m_mainWindow->pluginView(QStringLiteral("kateprojectplugin"));
    if (projectPlugin) {
        projectName = projectPlugin->property("projectName").toString();
    }

    auto formatText = [fileName, projectName](QString text) {
        text.replace("{FILENAME}", fileName);
        text.replace("{PROJECT}", projectName);
        return text.toUtf8();
    };

    QByteArray details = formatText(m_config->detailsText);
    QByteArray state = formatText(m_config->stateText);
    discordPresence.details = details;
    discordPresence.state = state;

    Discord_UpdatePresence(&discordPresence);
}

#include "discordrpcplugin.moc"
#include "moc_discordrpcplugin.cpp"
