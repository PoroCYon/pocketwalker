#pragma once
#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>

class IRSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IRSettingsDialog(QWidget* parent = nullptr);

private slots:
    void apply();

private:
    QComboBox* mode_combo;
    QLineEdit* host_edit;
    QSpinBox* port_spin;
};