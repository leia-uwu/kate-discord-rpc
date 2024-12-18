#include "discordrpcconfigpage.h"
#include "discordrpcplugin.h"

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

#include <KConfigGroup>
#include <KSharedConfig>

DiscordRpcConfigPage::DiscordRpcConfigPage(QWidget *parent, DiscordRpcPlugin *plugin)
    : KTextEditor::ConfigPage(parent)
    , m_plugin(plugin)
{
    auto layout = new QVBoxLayout(this);

    auto formatLabel = new QLabel(this);
    formatLabel->setText(
        "Format:\n\
{FILENAME} for current open file name\n\
{PROJECT} for current open project name");
    layout->addWidget(formatLabel);

    auto detailsLabel = new QLabel(this);
    detailsLabel->setText("Details:");
    layout->addWidget(detailsLabel);

    m_detailsLineEdit = new QLineEdit(this);
    connect(m_detailsLineEdit, &QLineEdit::textEdited, this, &DiscordRpcConfigPage::changed);
    layout->addWidget(m_detailsLineEdit);

    auto stateLabel = new QLabel(this);
    stateLabel->setText("State:");
    layout->addWidget(stateLabel);
    m_stateLineEdit = new QLineEdit(this);
    connect(m_stateLineEdit, &QLineEdit::textEdited, this, &DiscordRpcConfigPage::changed);
    layout->addWidget(m_stateLineEdit);

    m_showTimeCheckBox = new QCheckBox("Show elapsed time", this);
    layout->addWidget(m_showTimeCheckBox);
    connect(m_showTimeCheckBox, &QCheckBox::checkStateChanged, this, &DiscordRpcConfigPage::changed);

    layout->addStretch();

    reset();
}

QString DiscordRpcConfigPage::name() const
{
    return QStringLiteral("Discord RPC");
}

QString DiscordRpcConfigPage::fullName() const
{
    return QStringLiteral("Discord RPC Settings");
}

QIcon DiscordRpcConfigPage::icon() const
{
    return QIcon::fromTheme("dialog-messages");
}

void DiscordRpcConfigPage::apply()
{
    KConfigGroup config(KSharedConfig::openConfig(), QStringLiteral("DiscordRPC"));

    auto defauls = DiscordRpcPlugin::DefaultConfig;

    config.writeEntry("DetailsText", m_detailsLineEdit->text());
    config.writeEntry("StateText", m_stateLineEdit->text());
    config.writeEntry("ShowElapsedTime", m_showTimeCheckBox->isChecked());

    config.sync();
    m_plugin->readConfig();
    m_plugin->updateStatus();
}

void DiscordRpcConfigPage::reset()
{
    m_plugin->readConfig();

    m_detailsLineEdit->setText(m_plugin->m_config->detailsText);
    m_stateLineEdit->setText(m_plugin->m_config->stateText);
    m_showTimeCheckBox->setChecked(m_plugin->m_config->showElapsedTime);
}

void DiscordRpcConfigPage::defaults()
{
}
