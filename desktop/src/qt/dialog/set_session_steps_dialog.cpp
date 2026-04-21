#include "set_session_steps_dialog.h"

SetSessionStepsDialog::SetSessionStepsDialog(QWidget* parent) : QDialog(parent)
{
    setWindowTitle("Set Session Steps");

    auto* layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("Steps (0-9999999):", this));

    steps_spin_box = new QSpinBox(this);
    steps_spin_box->setRange(0, 9999999);
    steps_spin_box->setValue(0);
    layout->addWidget(steps_spin_box);

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttons);
}

uint32_t SetSessionStepsDialog::steps()
{
    return static_cast<uint32_t>(steps_spin_box->value());
}
