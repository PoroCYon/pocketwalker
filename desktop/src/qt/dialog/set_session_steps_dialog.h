#pragma once

#include <QDialog>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QLabel>

class SetSessionStepsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetSessionStepsDialog(QWidget* parent = nullptr);

    uint32_t steps();

private:
    QSpinBox* steps_spin_box;
};
