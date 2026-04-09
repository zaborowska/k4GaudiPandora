/**
 *  @file   DDMarlinPandora/include/DDGeometryCreatorODD.h
 *
 *  @brief  Header file for the geometry creator class.
 *
 *  $Log: $
 */

#ifndef DDGEOMETRYODD_CREATOR_H
#define DDGEOMETRYODD_CREATOR_H

#include "Api/PandoraApi.h"

#include "DDGeometryCreator.h"

//------------------------------------------------------------------------------------------------------------------------------------------

/**
 *  @brief  DDGeometryCreator class
 */
class DDGeometryCreatorODD : public DDGeometryCreator {
public:
  /**
   *  @brief  Constructor
   *
   *  @param  settings the creator settings
   *  @param  pPandora address of the relevant pandora instance
   */
  DDGeometryCreatorODD(const Settings& settings, pandora::Pandora& pPandora,
                           Gaudi::Algorithm* algorithm);

  /**
   *  @brief  Create geometry
   */
  pandora::StatusCode CreateGeometry() const override;

private:
  /**
   *  @brief  Set mandatory sub detector parameters
   *
   *  @param  subDetectorTypeMap the sub detector type map
   */
  void SetMandatorySubDetectorParameters(SubDetectorTypeMap& subDetectorTypeMap) const override;
};

#endif // #ifndef GEOMETRY_CREATOR_H
