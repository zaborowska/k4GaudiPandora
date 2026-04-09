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

#ifndef K4GAUDIPANDORA_ODD_PSEUDO_LAYER_PLUGIN_H
#define K4GAUDIPANDORA_ODD_PSEUDO_LAYER_PLUGIN_H 1

#include "Pandora/PandoraInputTypes.h"
#include "Plugins/PseudoLayerPlugin.h"

#include <vector>

class ODDPseudoLayerPlugin : public pandora::PseudoLayerPlugin {
public:
  ODDPseudoLayerPlugin() = default;

private:
  pandora::StatusCode Initialize() override;
  pandora::StatusCode ReadSettings(const pandora::TiXmlHandle) override;

public:
  unsigned int GetPseudoLayer(const pandora::CartesianVector& positionVector) const override;
  unsigned int GetPseudoLayerAtIp() const override;

private:
  using LayerPositionList = std::vector<float>;

  void StoreLayerPositions(const pandora::SubDetector& subDetector, LayerPositionList& layerPositions) const;
  static unsigned int FindMatchingLayer(float position, const LayerPositionList& layerPositions);
  static void SortAndUnique(LayerPositionList& layerPositions);

  LayerPositionList m_barrelLayerPositions;
  LayerPositionList m_endcapLayerPositions;
  float m_barrelInnerR = 0.f;
  float m_endcapInnerZ = 0.f;
};

#endif
