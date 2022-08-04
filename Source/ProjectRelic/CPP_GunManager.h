// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_WeaponManager.h"
#include "CPP_GunManager.generated.h"

/*****************************************************************************************************
 * Type: Class
 *
 * Name: CPP_GunManager
 *
 * Author: Jaber Ahmed
 *
 * Purpose: Parent for all gun style wewapons
 *
 * References: N/A
 *
 * See Also: N/A
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 04/08/2022    JA          v1.0        Created a parent gun manager 
*****************************************************************************************************/
UCLASS()
class PROJECTRELIC_API ACPP_GunManager : public ACPP_WeaponManager
{
	GENERATED_BODY()
private:
	int m_ammoCount; // Ammount of ammo weapon can hold
	int m_ammoRemaining; // Ammo of ammo remaining 
public:
	/*****************************************************************************
	  *   Function        : void
	  *   Purpose         :
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	ACPP_GunManager();
	/*****************************************************************************
	  *   Function        : void
	  *   Purpose         :
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	void SetTotalAmmoCount( int ammoCount );
	/*****************************************************************************
	  *   Function        : void
	  *   Purpose         :
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	int GetTotalAmmoCount();
	/*****************************************************************************
	  *   Function        : void
	  *   Purpose         :
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	void SetAmmoRemaining( int ammoRemaining );
	/*****************************************************************************
	  *   Function        : void
	  *   Purpose         :
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	int GetAmmoRemaining();


};
