#pragma once

#include <QDialog>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QLabel>

class SetWattsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetWattsDialog(QWidget* parent = nullptr);

    uint16_t watts();

private:
    QSpinBox* watt_spin_box;
};
