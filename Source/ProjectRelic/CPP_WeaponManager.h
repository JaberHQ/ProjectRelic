// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_WeaponManager.generated.h"
/********************************************************************************************************************
 * Type: Class
 *
 * Name: CPP_WeaponManager
 *
 * Author: Jaber Ahmed
 *
 * Purpose: Parent class for all weapons
 *
 * Functions: ACPP_WeaponManager(), virtual void BeginPlay() override, virtual void Tick( float DeltaTime ) override;
 *
 * References: N/A
 *
 * See Also: GunManager.h
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 11/07/2022    JA			 v1.0        Created a weapon manager
********************************************************************************************************************/
UCLASS()
class PROJECTRELIC_API ACPP_WeaponManager : public AActor
{
	GENERATED_BODY()
	
public:	
	/*****************************************************************************
	  *   Function        : ACPP_WeaponManager()
	  *   Purpose         : Constructor
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 11/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	ACPP_WeaponManager();
	  /*****************************************************************************
	  *   Function        : virtual void Tick( float DeltaTime ) override
	  *   Purpose         : Event tick
	  *   Parameters      : float DeltaTime
	  *   Returns         : N/A
	  *   Date altered    : 11/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	virtual void Tick( float DeltaTime ) override;
protected:
	/*****************************************************************************
	  *   Function        : virtual void BeginPlay() override
	  *   Purpose         : Begin play event
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 11/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	virtual void BeginPlay() override;

};
