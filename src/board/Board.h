/*
 * Copyright (C) 2021 Daniel Volk <mail@volkarts.com>
 *
 * This file is part of 6502emu - 6502 cycle correct emulator.
 *
 * 6502emu is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * You should have received a copy of the GNU General Public License
 * along with 6502emu. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "WireState.h"
#include <QObject>
#include <QScopedPointer>

class Bus;
class Clock;
class CPU;
class Device;
class UserState;
class QIODevice;

class Board : public QObject
{
    Q_OBJECT

public:
    explicit Board(QObject* parent = nullptr);
    ~Board() override;

    bool load(const QString& fileName);

    UserState* userState() const { return userState_.get(); }

    Bus* addressBus() const { return addressBus_; }
    Bus* dataBus() const { return dataBus_; }
    CPU* cpu() const { return cpu_; }
    Clock* clock() const { return clock_; }

    WireState rwLine() const { return rwLine_; }
    void setRwLine(WireState rwLine);

    WireState irqLine() const { return irqLine_; }
    void setIrqLine(WireState irqLine);

    WireState nmiLine() const { return nmiLine_; }
    void setNmiLine(WireState nmiLine);

    WireState resetLine() const { return resetLine_; }
    void setResetLine(WireState resetLine);

    WireState syncLine() const { return syncLine_; }
    void setSyncLine(WireState syncLine);

    const QList<Device*>& devices() const { return devices_; }
    Device* device(const QString& name);
    Bus* bus(const QString& name);

    Device* findDevice(uint16_t address);
    template<typename T>
    T* findDevice(uint16_t address)
    {
        auto* dev = findDevice(address);
        if (!dev)
            return nullptr;
        return qobject_cast<T*>(dev);
    }
    void clearDevices();

public slots:
    void startSingleInstructionStep();
    void stopSingleInstructionStep();

signals:
    void signalChanged();
    void clockEdge(StateEdge edge);
    void deviceListChanged();

protected:
    void childEvent(QChildEvent* event) override;

private slots:
    void recreateDeviceList();
    void onClockEdge(StateEdge edge);

private:
    QScopedPointer<UserState> userState_;
    Bus* addressBus_;
    Bus* dataBus_;
    WireState resetLine_;
    WireState rwLine_;
    WireState irqLine_;
    WireState nmiLine_;
    WireState syncLine_;

    CPU* cpu_;
    Clock* clock_;

    QList<Device*> devices_;
    bool deviceListReloadTriggered_;

    bool dbgSingleInstructionMode_;
};
