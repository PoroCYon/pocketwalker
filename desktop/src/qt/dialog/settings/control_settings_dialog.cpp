#include "control_settings_dialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QDialogButtonBox>

#include "desktop/src/qt/settings/app_settings.h"
#include "desktop/src/qt/settings/types/control_settings.h"

ControlSettingsDialog::ControlSettingsDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Control Settings");
    setMinimumWidth(400);

    auto& controls = AppSettings::instance.controls;

    auto make_button = [this](int key)
    {
        auto* btn = new KeyBindButton(this);
        btn->setKey(key);
        return btn;
    };

    left_button = make_button(controls.key_left);
    right_button = make_button(controls.key_right);
    center_button = make_button(controls.key_center);
    speedup_button = make_button(controls.key_speedup);
    synthetic_steps_button = make_button(controls.key_synthetic_steps_hold);

    auto* main_controls_form = new QFormLayout();
    main_controls_form->addRow("Left:", left_button);
    main_controls_form->addRow("Right:", right_button);
    main_controls_form->addRow("Center:", center_button);

    auto* extra_controls_form = new QFormLayout();
    extra_controls_form->addRow("Speedup (Hold):", speedup_button);
    extra_controls_form->addRow("Synthetic Steps (Hold):", synthetic_steps_button);

    auto* main_controls_group = new QGroupBox("Main Keys", this);
    main_controls_group->setLayout(main_controls_form);

    auto* extra_controls_group = new QGroupBox("Extra Keys", this);
    extra_controls_group->setLayout(extra_controls_form);

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(main_controls_group);
    layout->addWidget(extra_controls_group);
    layout->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, this, &ControlSettingsDialog::apply);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void ControlSettingsDialog::apply()
{
    auto& controls = AppSettings::instance.controls;

    controls.key_left = left_button->key();
    controls.key_right = right_button->key();
    controls.key_center = center_button->key();
    controls.key_speedup = speedup_button->key();
    controls.key_synthetic_steps_hold = synthetic_steps_button->key();

    accept();
}
