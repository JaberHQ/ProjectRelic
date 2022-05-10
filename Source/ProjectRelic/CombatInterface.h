// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"



// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/***************************************************************************************
 * Type: Class
 *
 * Name: CombatInterface
 *
 * Author: Jaber Ahmed
 *
 * Purpose: Interface for combat
 *
 * Functions: virtual bool CanTakedown(), virtual void Interact()
 *
 * References: N/A
 *
 * See Also: N/A
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 11/04/2022    JA          v1.0        Created an interface for melee takedown
 ***************************************************************************************/
class PROJECTRELIC_API ICombatInterface
{
	GENERATED_BODY()	
private:
public:
	/*****************************************************************************
	*   Function        : virtual bool CanTakedown() = 0
	*   Purpose         : Set if enemy can be taken down
	*   Parameters      : N/A
	*   Returns         : N/A
	*   Date altered    : 22/04/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
	*****************************************************************************/
	UFUNCTION()
		virtual bool CanTakedown() = 0;
	/*****************************************************************************
	*   Function        : virtual void Interact() = 0
	*   Purpose         : Interaction between Player and Enemy
	*   Parameters      : N/A
	*   Returns         : N/A
	*   Date altered    : 22/04/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
	*****************************************************************************/
	UFUNCTION()
		virtual void Interact() = 0;
	
};
