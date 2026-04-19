#include "ir_settings_dialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QLabel>

#include "desktop/src/qt/settings/app_settings.h"
#include "desktop/src/qt/settings/types/ir_settings.h"

IRSettingsDialog::IRSettingsDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("IR Settings");
    setMinimumWidth(400);

    auto& ir = AppSettings::instance.ir;

    mode_combo = new QComboBox(this);
    mode_combo->addItem("Client", static_cast<int>(IRSettings::Mode::Client));
    mode_combo->addItem("Server", static_cast<int>(IRSettings::Mode::Server));
    mode_combo->setCurrentIndex(static_cast<int>(ir.mode));

    host_edit = new QLineEdit(QString::fromStdString(ir.host), this);
    host_edit->setPlaceholderText("127.0.0.1");

    port_spin = new QSpinBox(this);
    port_spin->setRange(1, 65535);
    port_spin->setValue(ir.port);

    auto* connection_form = new QFormLayout();
    connection_form->addRow("Mode:", mode_combo);
    connection_form->addRow("Host:", host_edit);
    connection_form->addRow("Port:", port_spin);

    auto* connection_group = new QGroupBox("Network", this);
    connection_group->setLayout(connection_form);

    auto* note_text = new QLabel("Note: These settings require an emulation reset to take effect.");
    connection_form->addRow(note_text);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(connection_group);

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    layout->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, this, &IRSettingsDialog::apply);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void IRSettingsDialog::apply()
{
    auto& ir = AppSettings::instance.ir;

    ir.mode = static_cast<IRSettings::Mode>(mode_combo->currentData().toInt());
    ir.host = host_edit->text().toStdString();
    ir.port = port_spin->value();

    accept();
}