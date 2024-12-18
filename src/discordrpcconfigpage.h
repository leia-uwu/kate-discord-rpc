#include <KTextEditor/ConfigPage>

#include "discordrpcplugin.h"

class QLineEdit;
class QCheckBox;

class DiscordRpcConfigPage final : public KTextEditor::ConfigPage
{
public:
    explicit DiscordRpcConfigPage(QWidget *parent, DiscordRpcPlugin *plugin);

    QString name() const override;
    QString fullName() const override;
    QIcon icon() const override;

    void apply() override;
    void reset() override;
    void defaults() override;

private:
    DiscordRpcPlugin *const m_plugin;

    QLineEdit *m_detailsLineEdit = nullptr;
    QLineEdit *m_stateLineEdit = nullptr;

    QCheckBox *m_showTimeCheckBox = nullptr;
};
