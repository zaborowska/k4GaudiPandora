/**
 *  @file   DDMarlinPandora/src/DDGeometryCreator.cc
 *
 *  @brief  Implementation of the geometry creator class.
 *
 *  $Log: $
 */

#include "DDGeometryCreatorODD.h"

#include "DD4hep/DetType.h"
#include "DDRec/DetectorData.h"

#include <utility>

// Forward declarations. See DDPandoraPFANewProcessor.cc
//  dd4hep::rec::LayeredCalorimeterData * getExtension(std::string detectorName);
dd4hep::rec::LayeredCalorimeterData* getExtension(unsigned int includeFlag, unsigned int excludeFlag = 0);

std::vector<double> getTrackingRegionExtent();

DDGeometryCreatorODD::DDGeometryCreatorODD(const Settings& settings, pandora::Pandora& pPandora,
                                                   Gaudi::Algorithm* algorithm)
    : DDGeometryCreator(settings, pPandora, algorithm) {}

//------------------------------------------------------------------------------------------------------------------------------------------

pandora::StatusCode DDGeometryCreatorODD::CreateGeometry() const {
  SubDetectorTypeMap subDetectorTypeMap;
  this->SetMandatorySubDetectorParameters(subDetectorTypeMap);

  for (SubDetectorTypeMap::const_iterator iter = subDetectorTypeMap.begin(), iterEnd = subDetectorTypeMap.end();
       iter != iterEnd; ++iter) {
    PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=,
                             PandoraApi::Geometry::SubDetector::Create(m_pPandora, iter->second));
  }

  return pandora::STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

void DDGeometryCreatorODD::SetMandatorySubDetectorParameters(SubDetectorTypeMap& subDetectorTypeMap) const {
  PandoraApi::Geometry::SubDetector::Parameters eCalBarrelParameters, eCalEndCapParameters, hCalBarrelParameters,
      hCalEndCapParameters;
  this->SetDefaultSubDetectorParameters(
      *const_cast<dd4hep::rec::LayeredCalorimeterData*>(
          getExtension((dd4hep::DetType::CALORIMETER | dd4hep::DetType::ELECTROMAGNETIC | dd4hep::DetType::BARREL),
                       (dd4hep::DetType::AUXILIARY | dd4hep::DetType::FORWARD))),
      "ECalBarrel", pandora::ECAL_BARREL, eCalBarrelParameters);
  this->SetDefaultSubDetectorParameters(
      *const_cast<dd4hep::rec::LayeredCalorimeterData*>(
          getExtension((dd4hep::DetType::CALORIMETER | dd4hep::DetType::ELECTROMAGNETIC | dd4hep::DetType::ENDCAP),
                       (dd4hep::DetType::AUXILIARY | dd4hep::DetType::FORWARD))),
      "ECalEndCap", pandora::ECAL_ENDCAP, eCalEndCapParameters);
  this->SetDefaultSubDetectorParameters(
      *const_cast<dd4hep::rec::LayeredCalorimeterData*>(
          getExtension((dd4hep::DetType::CALORIMETER | dd4hep::DetType::HADRONIC | dd4hep::DetType::BARREL),
                       (dd4hep::DetType::AUXILIARY | dd4hep::DetType::FORWARD))),
      "HCalBarrel", pandora::HCAL_BARREL, hCalBarrelParameters);
  this->SetDefaultSubDetectorParameters(
      *const_cast<dd4hep::rec::LayeredCalorimeterData*>(
          getExtension((dd4hep::DetType::CALORIMETER | dd4hep::DetType::HADRONIC | dd4hep::DetType::ENDCAP),
                       (dd4hep::DetType::AUXILIARY | dd4hep::DetType::FORWARD))),
      "HCalEndCap", pandora::HCAL_ENDCAP, hCalEndCapParameters);

  subDetectorTypeMap[pandora::ECAL_BARREL] = eCalBarrelParameters;
  subDetectorTypeMap[pandora::ECAL_ENDCAP] = eCalEndCapParameters;
  subDetectorTypeMap[pandora::HCAL_BARREL] = hCalBarrelParameters;
  subDetectorTypeMap[pandora::HCAL_ENDCAP] = hCalEndCapParameters;

  // FIXME! AD: currently ignoring tracker parameters since we are using truth tracks
  /*
  PandoraApi::Geometry::SubDetector::Parameters trackerParameters;

  trackerParameters.m_subDetectorName = "Tracker";
  trackerParameters.m_subDetectorType = pandora::INNER_TRACKER;
  trackerParameters.m_innerRCoordinate = getTrackingRegionExtent()[0];
  trackerParameters.m_innerZCoordinate = 0.f;
  trackerParameters.m_innerPhiCoordinate = 0.f;
  trackerParameters.m_innerSymmetryOrder = 0;
  trackerParameters.m_outerRCoordinate = getTrackingRegionExtent()[1];
  trackerParameters.m_outerZCoordinate = getTrackingRegionExtent()[2];
  trackerParameters.m_outerPhiCoordinate = 0.f;
  trackerParameters.m_outerSymmetryOrder = 0;
  trackerParameters.m_isMirroredInZ = true;
  trackerParameters.m_nLayers = 0;
  subDetectorTypeMap[pandora::INNER_TRACKER] = trackerParameters;
  */

}
