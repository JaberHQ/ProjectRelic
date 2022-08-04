// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_Pickup.h"
#include "CPP_AmmoPickup.generated.h"

/*****************************************************************************************************
 * Type: Class
 *
 * Name: CPP_AmmoPickup
 *
 * Author: Jaber Ahmed
 *
 * Purpose: Increase ammo on overlap 
 *
 * References: N/A
 *
 * See Also: CPP_PlayerManager
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 25/07/2022    JA          v1.0		 Created pickup 
*****************************************************************************************************/
UCLASS()
class PROJECTRELIC_API ACPP_AmmoPickup : public ACPP_Pickup
{
	GENERATED_BODY()
private:
public:
	virtual void BeginPlay() override;
	/******************************************************************************************************************************************************************
	 *   Function        : virtual void OnBoxBeginOverlap( UPrimitiveComponent* OverlappedComp,
	 *						AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult ); override
	 *   Purpose         : Overlap object
	 *   Parameters      : PrimitiveComponent* OverlappedComp,
	 *						AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult
	 *   Returns         : N/A
	 *   Date altered    :
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
	******************************************************************************************************************************************************************/
	virtual void OnBoxBeginOverlap( UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult ) override;
public:
	ACPP_AmmoPickup();
};
