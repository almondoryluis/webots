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

#include "WbTrackWheel.hpp"

#include "WbFieldChecker.hpp"
#include "WbSFBool.hpp"
#include "WbSFDouble.hpp"
#include "WbSFVector2.hpp"

void WbTrackWheel::init() {
  mPosition = findSFVector2("position");
  mRadius = findSFDouble("radius");
  mInner = findSFBool("inner");

  // define Pose fields
  mTranslation = new WbSFVector3(WbVector3());
  mRotation = findSFRotation("rotation");
  mTranslationStep = new WbSFDouble(0.1);
  mRotationStep = new WbSFDouble(0.1);
}

WbTrackWheel::WbTrackWheel(WbTokenizer *tokenizer) : WbPose("TrackWheel", tokenizer) {
  init();
}

WbTrackWheel::WbTrackWheel(const WbTrackWheel &other) : WbPose(other) {
  init();
}

WbTrackWheel::WbTrackWheel(const WbNode &other) : WbPose(other) {
  init();
}

WbTrackWheel::~WbTrackWheel() {
}

void WbTrackWheel::preFinalize() {
  WbPose::preFinalize();

  updatePosition();
  updateRadius();
}

void WbTrackWheel::postFinalize() {
  WbPose::postFinalize();

  connect(mPosition, &WbSFVector2::changed, this, &WbTrackWheel::updatePosition);
  connect(mRadius, &WbSFDouble::changed, this, &WbTrackWheel::updateRadius);
  connect(mInner, &WbSFBool::changed, this, &WbTrackWheel::changed);

  emit changed();  // trigger WbTrack belt update
}

void WbTrackWheel::updatePosition() {
  setTranslation(mPosition->x(), 0, mPosition->y());
  updateTranslation();
  if (isPostFinalizedCalled())
    emit changed();
}

void WbTrackWheel::updateRadius() {
  if (WbFieldChecker::resetDoubleIfNonPositive(this, mRadius, 0.1) && isPostFinalizedCalled())
    emit changed();
}

void WbTrackWheel::rotate(double traveledDistance) {
  double angle = traveledDistance / radius();
  if (mInner->value())
    angle = -angle;

  WbRotation newRotation(WbMatrix3(0, -1, 0, angle) * rotation().toMatrix3());
  newRotation.normalize();
  setRotation(newRotation);
  updateRotation();
}

bool WbTrackWheel::shallExport() const {
  return true;
}

void WbTrackWheel::write(WbWriter &writer) const {
  if (writer.isUrdf())
    return;
  WbPose::write(writer);
}

QStringList WbTrackWheel::fieldsToSynchronizeWithW3d() const {
  QStringList fields;
  fields << "position"
         << "radius"
         << "rotation"
         << "inner";
  return fields;
}

void WbTrackWheel::exportNodeFields(WbWriter &writer) const {
  WbBaseNode::exportNodeFields(writer);

  if (writer.isW3d())
    writer << " rotation=\'" << mRotation->value() << "\'";
}
