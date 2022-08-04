// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_GunManager.h"
#include "CPP_AssaultRifle.generated.h"

/*****************************************************************************************************
 * Type: Class
 *
 * Name: CPP_AssaultRifle
 *
 * Author: Jaber Ahmed
 *
 * Purpose: Assault rifle weapon
 *
 * References: N/A
 *
 * See Also: CPP_CharacterManager
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 04/08/2022    JA          v1.0        Created an assault rifle object
*****************************************************************************************************/
UCLASS()
class PROJECTRELIC_API ACPP_AssaultRifle : public ACPP_GunManager
{
	GENERATED_BODY()
private:
public:
	/*****************************************************************************
	  *   Function        : ACPP_AssaultRifle()
	  *   Purpose         : Constructor
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	ACPP_AssaultRifle();
};
