#include "set_watts_dialog.h"

SetWattsDialog::SetWattsDialog(QWidget* parent) : QDialog(parent)
{
    setWindowTitle("Set Watts");

    auto *layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("Watts (0-65535):", this));

    watt_spin_box = new QSpinBox(this);
    watt_spin_box->setRange(0, 65535);
    watt_spin_box->setValue(0);
    layout->addWidget(watt_spin_box);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttons);
}

uint16_t SetWattsDialog::watts()
{
    return static_cast<uint16_t>(watt_spin_box->value());
}
