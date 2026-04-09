/*
 * Copyright (c) 2020-2024 Key4hep-Project.
 *
 * This file is part of Key4hep.
 * See https://key4hep.github.io/key4hep-doc/ for further info.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ODDPseudoLayerPlugin.h"

#include "Helpers/XmlHelper.h"
#include "Geometry/SubDetector.h"
#include "Managers/GeometryManager.h"
#include "Pandora/Pandora.h"

#include <algorithm>
#include <cmath>

namespace {
constexpr float layerTolerance = 1e-3f;
}

pandora::StatusCode ODDPseudoLayerPlugin::Initialize() {
  const auto* geometry = this->GetPandora().GetGeometry();
  if (!geometry) {
    return pandora::STATUS_CODE_NOT_INITIALIZED;
  }

  this->StoreLayerPositions(geometry->GetSubDetector(pandora::ECAL_BARREL), m_barrelLayerPositions);
  this->StoreLayerPositions(geometry->GetSubDetector(pandora::HCAL_BARREL), m_barrelLayerPositions);
  this->StoreLayerPositions(geometry->GetSubDetector(pandora::ECAL_ENDCAP), m_endcapLayerPositions);
  this->StoreLayerPositions(geometry->GetSubDetector(pandora::HCAL_ENDCAP), m_endcapLayerPositions);

  SortAndUnique(m_barrelLayerPositions);
  SortAndUnique(m_endcapLayerPositions);

  if (m_barrelLayerPositions.empty() && m_endcapLayerPositions.empty()) {
    return pandora::STATUS_CODE_NOT_INITIALIZED;
  }

  m_barrelInnerR = geometry->GetSubDetector(pandora::ECAL_BARREL).GetInnerRCoordinate();
  m_endcapInnerZ = geometry->GetSubDetector(pandora::ECAL_ENDCAP).GetInnerZCoordinate();

  return pandora::STATUS_CODE_SUCCESS;
}

pandora::StatusCode ODDPseudoLayerPlugin::ReadSettings(const pandora::TiXmlHandle) {
  return pandora::STATUS_CODE_SUCCESS;
}

unsigned int ODDPseudoLayerPlugin::GetPseudoLayer(const pandora::CartesianVector& positionVector) const {
  const float r = std::hypot(positionVector.GetX(), positionVector.GetY());
  const float z = std::fabs(positionVector.GetZ());

  const float barrelDepth = r - m_barrelInnerR;
  const float endcapDepth = z - m_endcapInnerZ;

  bool useBarrel = !m_barrelLayerPositions.empty();
  if (m_barrelLayerPositions.empty()) {
    useBarrel = false;
  } else if (m_endcapLayerPositions.empty()) {
    useBarrel = true;
  } else if (barrelDepth >= 0.f && endcapDepth >= 0.f) {
    useBarrel = (barrelDepth <= endcapDepth);
  } else if (endcapDepth >= 0.f) {
    useBarrel = false;
  }

  return useBarrel ? FindMatchingLayer(r, m_barrelLayerPositions) : FindMatchingLayer(z, m_endcapLayerPositions);
}

unsigned int ODDPseudoLayerPlugin::GetPseudoLayerAtIp() const {
  return 0;
}

void ODDPseudoLayerPlugin::StoreLayerPositions(const pandora::SubDetector& subDetector,
                                               LayerPositionList& layerPositions) const {
  for (const auto& layer : subDetector.GetSubDetectorLayerVector()) {
    layerPositions.push_back(layer.GetClosestDistanceToIp());
  }
}

unsigned int ODDPseudoLayerPlugin::FindMatchingLayer(float position, const LayerPositionList& layerPositions) {
  if (layerPositions.empty()) {
    return 0;
  }

  const auto iter = std::lower_bound(layerPositions.begin(), layerPositions.end(), position);
  if (iter == layerPositions.begin()) {
    return 0;
  }
  if (iter == layerPositions.end()) {
    return static_cast<unsigned int>(layerPositions.size() - 1);
  }

  const auto upperIndex = static_cast<unsigned int>(std::distance(layerPositions.begin(), iter));
  const auto lowerIndex = upperIndex - 1;
  const auto lowerDistance = std::fabs(position - layerPositions[lowerIndex]);
  const auto upperDistance = std::fabs(layerPositions[upperIndex] - position);
  return (lowerDistance <= upperDistance) ? lowerIndex : upperIndex;
}

void ODDPseudoLayerPlugin::SortAndUnique(LayerPositionList& layerPositions) {
  std::sort(layerPositions.begin(), layerPositions.end());
  layerPositions.erase(std::unique(layerPositions.begin(), layerPositions.end(),
                                   [](float lhs, float rhs) { return std::fabs(lhs - rhs) < layerTolerance; }),
                       layerPositions.end());
}
