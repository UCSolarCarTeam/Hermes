/**
 *  Schulich Delta Hermes
 *  Copyright (C) 2015 University of Calgary Solar Car Team
 *
 *  This file is part of Schulich Delta Hermes
 *
 *  Schulich Delta Hermes is free software:
 *  you can redistribute it and/or modify it under the terms
 *  of the GNU Affero General Public License as published by
 *  the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  Schulich Delta Hermes is distributed
 *  in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General
 *  Public License along with Schulich Delta Hermes.
 *  If not, see <http://www.gnu.org/licenses/>.
 *
 *  For further contact, email <software@calgarysolarcar.ca>
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <QTest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QScopedPointer>
#include <QDebug>

#include "BusinessLayer/JsonForwarder/JsonForwarder.h"
#include "BusinessLayer/JsonForwarder/JsonDefines.h"

#include "Tests/CommunicationLayer/CommDeviceControl/MockMessageForwarder.h"
#include "Tests/DataLayer/BatteryData/MockBatteryData.h"
#include "Tests/DataLayer/FaultsData/MockFaultsData.h"
#include "Tests/DataLayer/PowerData/MockPowerData.h"
#include "Tests/DataLayer/VehicleData/MockVehicleData.h"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;

namespace
{
    const int FORWARD_INTERVAL_MSEC = 50;
    const int FORWARD_ITERATIONS = 10;
}

class JsonForwarderTest : public ::testing::Test
{

protected:
    QScopedPointer<MockBatteryData> batteryData_;
    QScopedPointer<MockFaultsData> faultsData_;
    QScopedPointer<MockPowerData> powerData_;
    QScopedPointer<MockVehicleData> vehicleData_;
    QScopedPointer<MockMessageForwarder> messageForwarder_;
    QScopedPointer<JsonForwarder> jsonForwarder_;

    virtual void SetUp()
    {
        batteryData_.reset(new MockBatteryData());
        faultsData_.reset(new MockFaultsData());
        powerData_.reset(new MockPowerData());
        vehicleData_.reset(new MockVehicleData());
        messageForwarder_.reset(new MockMessageForwarder());
        jsonForwarder_.reset(new JsonForwarder(*batteryData_,
                                               *faultsData_,
                                               *powerData_,
                                               *vehicleData_,
                                               *messageForwarder_));

        QList<double> emptyList = {0, 0, 0, 0, 0, 0, 0, 0};
        EXPECT_CALL(*batteryData_, mod0PcbTemperature())
            .WillRepeatedly(Return(0));
        EXPECT_CALL(*batteryData_, mod0CellTemperature())
            .WillRepeatedly(Return(0));
        EXPECT_CALL(*batteryData_, mod0CellVoltages())
            .WillRepeatedly(Return(emptyList));
        EXPECT_CALL(*batteryData_, mod1PcbTemperature())
            .WillRepeatedly(Return(0));
        EXPECT_CALL(*batteryData_, mod1CellTemperature())
            .WillRepeatedly(Return(0));
        EXPECT_CALL(*batteryData_, mod1CellVoltages())
            .WillRepeatedly(Return(emptyList));
        EXPECT_CALL(*batteryData_, mod2PcbTemperature())
            .WillRepeatedly(Return(0));
        EXPECT_CALL(*batteryData_, mod2CellTemperature())
            .WillRepeatedly(Return(0));
        EXPECT_CALL(*batteryData_, mod2CellVoltages())
            .WillRepeatedly(Return(emptyList));
        EXPECT_CALL(*batteryData_, mod3PcbTemperature())
            .WillRepeatedly(Return(0));
        EXPECT_CALL(*batteryData_, mod3CellTemperature())
            .WillRepeatedly(Return(0));
        EXPECT_CALL(*batteryData_, mod3CellVoltages())
            .WillRepeatedly(Return(emptyList));
        EXPECT_CALL(*batteryData_, batteryVoltage())
            .WillRepeatedly(Return(0));
        EXPECT_CALL(*batteryData_, batteryCurrent())
            .WillRepeatedly(Return(0));

        EXPECT_CALL(*faultsData_, motorOneFaults())
            .WillRepeatedly(Return(MotorFaults(0)));
        EXPECT_CALL(*faultsData_, motorOneLimitFlags())
            .WillRepeatedly(Return(LimitFlags(0)));
        EXPECT_CALL(*faultsData_, motorTwoFaults())
            .WillRepeatedly(Return(MotorFaults(0)));
        EXPECT_CALL(*faultsData_, motorTwoLimitFlags())
            .WillRepeatedly(Return(LimitFlags(0)));
        EXPECT_CALL(*faultsData_, batteryFaults())
            .WillRepeatedly(Return(BatteryFaults(0)));

        EXPECT_CALL(*powerData_, busCurrentA())
            .WillRepeatedly(Return(0));
        EXPECT_CALL(*powerData_, busVoltage())
            .WillRepeatedly(Return(0));
        EXPECT_CALL(*powerData_, motorVoltageReal())
            .WillRepeatedly(Return(0));
        EXPECT_CALL(*powerData_, motorCurrentReal())
            .WillRepeatedly(Return(0));
        EXPECT_CALL(*powerData_, backEmfImaginary())
            .WillRepeatedly(Return(0));
        EXPECT_CALL(*powerData_, dcBusAmpHours())
            .WillRepeatedly(Return(0));

        EXPECT_CALL(*vehicleData_, driverSetSpeedMetersPerSecond())
            .WillRepeatedly(Return(0));
        EXPECT_CALL(*vehicleData_, driverSetCurrent())
            .WillRepeatedly(Return(0));
        EXPECT_CALL(*vehicleData_, vehicleVelocityMetersPerSecond())
            .WillRepeatedly(Return(0));
        EXPECT_CALL(*vehicleData_, motorVelocityRpm())
            .WillRepeatedly(Return(0));
        EXPECT_CALL(*vehicleData_, ipmHeatSinkTemp())
            .WillRepeatedly(Return(0));
        EXPECT_CALL(*vehicleData_, dspBoardTemp())
            .WillRepeatedly(Return(0));
        EXPECT_CALL(*vehicleData_, receivedErrorCount())
            .WillRepeatedly(Return(0));
        EXPECT_CALL(*vehicleData_, transmittedErrorCount())
            .WillRepeatedly(Return(0));
    }
};

/****** TESTS ******/

TEST_F(JsonForwarderTest, dataForwarded)
{
    EXPECT_CALL(*messageForwarder_, forwardData(_))
        .Times(FORWARD_ITERATIONS);
    jsonForwarder_->startForwardingData(FORWARD_INTERVAL_MSEC);
    QTest::qWait(FORWARD_INTERVAL_MSEC * FORWARD_ITERATIONS + FORWARD_INTERVAL_MSEC/2);
}