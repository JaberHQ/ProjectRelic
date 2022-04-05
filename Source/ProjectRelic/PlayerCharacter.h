// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterManager.h"

#include "PlayerCharacter.generated.h"

/*************************************************************************************************
 * Type: Class
 *
 * Name: PlayerCharacter
 *
 * Author: Jaber Ahmed
 *
 * Purpose: Child class for the player 
 *
 * Functions: APlayerCharacter(), ~APlayerCharacter(), 
 *   virtual void SetupPlayerInputComponent( class UInputComponent* playerInputComponent ) override;
 *
 * References:
 *
 * See Also:
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 15/03/2022    JA			 1.0         Prototype version
 **************************************************************************************************/
UCLASS()
class PROJECTRELIC_API APlayerCharacter : public ACharacterManager
{
	GENERATED_BODY()

public:
	/********************************************************
	   *   Function        :
	   *   Purpose         :
	   *   Parameters      :
	   *   Returns         :
	   *   Date altered    :
	   *   Contributors    : Jaber Ahmed
	   *   Notes           :
	   *   See also        :
	*********************************************************/
	APlayerCharacter();
	/********************************************************
	   *   Function        :
	   *   Purpose         :
	   *   Parameters      :
	   *   Returns         :
	   *   Date altered    :
	   *   Contributors    : Jaber Ahmed
	   *   Notes           :
	   *   See also        :
	*********************************************************/
	virtual void SetupPlayerInputComponent( class UInputComponent* playerInputComponent ) override;

};
