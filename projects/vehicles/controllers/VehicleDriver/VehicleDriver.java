// Copyright 1996-2024 Cyberbotics Ltd.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

import com.cyberbotics.webots.controller.Robot;
import com.cyberbotics.webots.controller.vehicle.Driver;

public class VehicleDriver {
  public static void main(String[] args) {
    Driver driver = new Driver();
    driver.setCruisingSpeed(20.0);

    while (driver.step() != -1) {
      double angle = 0.3 * Math.cos(driver.getTime());
      driver.setSteeringAngle(angle);
    };
  }
}
