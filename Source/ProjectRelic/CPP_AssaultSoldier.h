// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_AIManager.h"
#include "CPP_AssaultSoldier.generated.h"

/**************************************************************************************
 * Type: Class
 *
 * Name: CPP_AssaultSoldier
 *
 * Author: Jaber Ahmed
 *
 * Purpose: AI with an Assault Rifle 
 *
 * Functions: N/A
 *
 * References: N/A
 *
 * See Also: CPP_AIManager.h, EnemyCharacter.h
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 02/05/2022    JA          v1.0        Created a new type of AI for AR's
 * 18/05/2022    JA			 v1.1		 Set default walk speed, got animations working
 **************************************************************************************/
UCLASS()
class PROJECTRELIC_API ACPP_AssaultSoldier : public ACPP_AIManager
{
	GENERATED_BODY()
	
private:
	float m_defaultWalkSpeed; // Default walk speed
public:
	/*****************************************************************************
	 *   Function        : ACPP_AssaultSoldier()
	 *   Purpose         : Constructor
	 *   Parameters      : FObjectInitializer const& objectInitalizer
	 *   Returns         : N/A
	 *   Date altered    : 18/05/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
	*****************************************************************************/
	ACPP_AssaultSoldier();
	/*****************************************************************************
	 *   Function        : virtual void BeginPlay() override
	 *   Purpose         : BeginPlay event
	 *   Parameters      : N/A
	 *   Returns         : N/A
	 *   Date altered    : 02/05/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
	*****************************************************************************/
	virtual void BeginPlay() override;

	
};
