// Copyright 2016 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <QCheckBox>
#include <QComboBox>
#include <QObject>
#include <QString>
#include "core/settings.h"
#include "yuzu/configuration/configuration_shared.h"
#include "yuzu/configuration/configure_per_game.h"

namespace ConfigurationShared {
Trackers trackers = {};
}

void ConfigurationShared::ApplyPerGameSetting(Settings::Setting<bool>* setting,
                                              const QCheckBox* checkbox) {
    if (checkbox->checkState() == Qt::PartiallyChecked) {
        setting->SetGlobal(true);
    } else {
        setting->SetGlobal(false);
        setting->SetValue(checkbox->checkState() == Qt::Checked);
    }
}

void ConfigurationShared::ApplyPerGameSetting(Settings::Setting<bool>* setting,
                                              const QCheckBox* checkbox,
                                              const CheckState& tracker) {
    if (tracker == CheckState::Global) {
        setting->SetGlobal(true);
    } else {
        setting->SetGlobal(false);
        setting->SetValue(checkbox->checkState());
    }
}

void ConfigurationShared::ApplyPerGameSetting(Settings::Setting<int>* setting,
                                              const QComboBox* combobox) {
    if (combobox->currentIndex() == ConfigurationShared::USE_GLOBAL_INDEX) {
        setting->SetGlobal(true);
    } else {
        setting->SetGlobal(false);
        setting->SetValue(combobox->currentIndex() - ConfigurationShared::USE_GLOBAL_OFFSET);
    }
}

void ConfigurationShared::ApplyPerGameSetting(Settings::Setting<Settings::RendererBackend>* setting,
                                              const QComboBox* combobox) {
    if (combobox->currentIndex() == ConfigurationShared::USE_GLOBAL_INDEX) {
        setting->SetGlobal(true);
    } else {
        setting->SetGlobal(false);
        setting->SetValue(static_cast<Settings::RendererBackend>(
            combobox->currentIndex() - ConfigurationShared::USE_GLOBAL_OFFSET));
    }
}

void ConfigurationShared::SetPerGameSetting(QCheckBox* checkbox,
                                            const Settings::Setting<bool>* setting) {
    if (setting->UsingGlobal()) {
        checkbox->setCheckState(Qt::PartiallyChecked);
    } else {
        checkbox->setCheckState(setting->GetValue() ? Qt::Checked : Qt::Unchecked);
    }
}

void ConfigurationShared::SetPerGameSetting(QComboBox* combobox,
                                            const Settings::Setting<int>* setting) {
    combobox->setCurrentIndex(setting->UsingGlobal()
                                  ? ConfigurationShared::USE_GLOBAL_INDEX
                                  : setting->GetValue() + ConfigurationShared::USE_GLOBAL_OFFSET);
}

void ConfigurationShared::SetPerGameSetting(
    QComboBox* combobox, const Settings::Setting<Settings::RendererBackend>* setting) {
    combobox->setCurrentIndex(setting->UsingGlobal() ? ConfigurationShared::USE_GLOBAL_INDEX
                                                     : static_cast<int>(setting->GetValue()) +
                                                           ConfigurationShared::USE_GLOBAL_OFFSET);
}

void ConfigurationShared::SetPerGameSetting(
    QComboBox* combobox, const Settings::Setting<Settings::GPUAccuracy>* setting) {
    combobox->setCurrentIndex(setting->UsingGlobal() ? ConfigurationShared::USE_GLOBAL_INDEX
                                                     : static_cast<int>(setting->GetValue()) +
                                                           ConfigurationShared::USE_GLOBAL_OFFSET);
}

void ConfigurationShared::SetHighlight(QWidget* widget, const std::string& name, bool highlighted) {
    if (highlighted) {
        widget->setStyleSheet(
            QStringLiteral("QWidget#%1 { background-color:rgba(0,203,255,0.5) }")
                .arg(QString::fromStdString(name)));
    } else {
        widget->setStyleSheet(
            QStringLiteral("QWidget#%1 { background-color:rgba(0,0,0,0) }")
                .arg(QString::fromStdString(name)));
    }
    widget->show();
}

void ConfigurationShared::SetColoredTristate(QCheckBox* checkbox, const std::string& name,
                                             const Settings::Setting<bool>& setting,
                                             ConfigurationShared::CheckState& tracker) {
    if (setting.UsingGlobal()) {
        tracker = CheckState::Global;
    } else {
        tracker = (setting.GetValue() == setting.GetValue(true)) ? CheckState::On : CheckState::Off;
    }
    SetHighlight(checkbox, name, tracker != CheckState::Global);
    QObject::connect(
        checkbox, &QCheckBox::clicked, checkbox, [checkbox, name, setting, &tracker]() {
            tracker =
                static_cast<ConfigurationShared::CheckState>((tracker + 1) % CheckState::Count);
            if (tracker == CheckState::Global) {
                checkbox->setChecked(setting.GetValue(true));
            }
            SetHighlight(checkbox, name, tracker != CheckState::Global);
        });
}

void ConfigurationShared::InsertGlobalItem(QComboBox* combobox) {
    const QString use_global_text = ConfigurePerGame::tr("Use global configuration");
    combobox->insertItem(ConfigurationShared::USE_GLOBAL_INDEX, use_global_text);
    combobox->insertSeparator(ConfigurationShared::USE_GLOBAL_SEPARATOR_INDEX);
}

void ConfigurationShared::InsertGlobalItem(QComboBox* combobox, int global_index) {
    const QString use_global_text = ConfigurePerGame::tr("Use global configuration (%1)").arg(combobox->itemText(global_index));
    combobox->insertItem(ConfigurationShared::USE_GLOBAL_INDEX, use_global_text);
    combobox->insertSeparator(ConfigurationShared::USE_GLOBAL_SEPARATOR_INDEX);
}
