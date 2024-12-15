#include "discordrpcplugin.h"

#include <KPluginFactory>
#include <QDebug>

#include <KTextEditor/Document>
#include <KTextEditor/MainWindow>
#include <KTextEditor/Plugin>
#include <KTextEditor/View>
#include <discord_rpc.h>
#include <ktexteditor/document.h>

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
}

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

    discordPresence.startTimestamp = m_startTimestamp;

    QString fileName = "";
    KTextEditor::View *view = m_mainWindow->activeView();
    if (view) {
        fileName = "Editing " + view->document()->url().fileName();
    }

    auto detailsArr = fileName.toUtf8();
    detailsArr.truncate(128);
    discordPresence.details = detailsArr;

    QString projectName = "";
    QObject *projectPlugin = m_mainWindow->pluginView(QStringLiteral("kateprojectplugin"));
    if (projectPlugin) {
        projectName = "Project: " + projectPlugin->property("projectName").toString();
    }

    auto stateStr = projectName.toUtf8();
    stateStr.truncate(128);
    discordPresence.state = stateStr;

    Discord_UpdatePresence(&discordPresence);
}

#include "discordrpcplugin.moc"
#include "moc_discordrpcplugin.cpp"
