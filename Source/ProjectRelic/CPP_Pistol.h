// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_GunManager.h"
#include "CPP_Pistol.generated.h"

/*****************************************************************************************************
 * Type: Class
 *
 * Name: CPP_Pistol
 *
 * Author: Jaber Ahmed
 *
 * Purpose: Pistol child actor
 *
 * Functions: ACPP_Pistol
 *
 * References: N/A
 *
 * See Also: CPP_GunManager, CPP_PlayerManager, CPP_AIManager, CPP_CharacterManager
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 25/07/2022    JA          v1.0		 Created a pistol
*****************************************************************************************************/
UCLASS()
class PROJECTRELIC_API ACPP_Pistol : public ACPP_GunManager
{
	GENERATED_BODY()
private:
public:
/*****************************************************************************
  *   Function        : ACPP_Pistol();
  *   Purpose         : Constructor
  *   Parameters      : N/A
  *   Returns         : N/A
  *   Date altered    : 26/07/2022
  *   Contributors    : Jaber Ahmed
  *   Notes           : N/A
  *   See also        : N/A           
*****************************************************************************/
	ACPP_Pistol();
};
