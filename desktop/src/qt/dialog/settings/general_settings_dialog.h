#pragma once
#include <QDialog>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>

class GeneralSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GeneralSettingsDialog(QWidget* parent = nullptr);

signals:
    void themeChanged();

private slots:
    void onBootOnLaunchToggled(bool enabled);
    void onBrowseRom();
    void apply();

private:
    QComboBox* theme_combo;
    QCheckBox* boot_on_launch_check;
    QLineEdit* default_rom_edit;
    QPushButton* browse_rom_btn;
};
