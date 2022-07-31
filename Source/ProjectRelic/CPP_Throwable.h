// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CPP_Throwable.generated.h"
/********************************************************************************************************************
 * Type: Class
 *
 * Name: CPP_Throwable
 *
 * Author: Jaber Ahmed
 *
 * Purpose: Parent class for throwable objects
 *
 * Functions: ACPP_Throwable, BeginPlay, Tick( float DeltaTime ), ThrowObject( FVector forwardVector )
 *
 * References: N/A
 *
 * See Also: CPP_PlayerManager
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 25/07/2022    JA			 v1.0        Created a throwable object
********************************************************************************************************************/
UCLASS()
class PROJECTRELIC_API ACPP_Throwable : public AActor
{
	GENERATED_BODY()
private:
	FVector m_velocity;

public:
	UPROPERTY( EditDefaultsOnly, Category = "Throwable" )
		class USkeletalMeshComponent* throwableMeshComp;

public:	
	/*****************************************************************************
	*   Function        : ACPP_Throwable()
	*   Purpose         : Constructor
	*   Parameters      : N/A
	*   Returns         : N/A
	*   Date altered    : 26/07/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A           
	*****************************************************************************/
	ACPP_Throwable();
	/*****************************************************************************
	*   Function        : virtual void Tick( float DeltaTime ) override
	*   Purpose         : Event tick
	*   Parameters      : float DeltaTime
	*   Returns         : N/A
	*   Date altered    : 26/07/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A           
	*****************************************************************************/
	virtual void Tick( float DeltaTime ) override;
	/*****************************************************************************
	*   Function        : void ThrowObject( FVector forwardVector )
	*   Purpose         : Impulse to throw object
	*   Parameters      : FVector forwardVector
	*   Returns         : N/A
	*   Date altered    : 26/07/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
	*****************************************************************************/
	void ThrowObject( FVector forwardVector, FVector playerLocation );

protected:
	/*****************************************************************************
	*   Function        : virtual void BeginPlay() override
	*   Purpose         : Begin play event
	*   Parameters      : N/A
	*   Returns         : N/A
	*   Date altered    : 26/07/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
    *****************************************************************************/
	virtual void BeginPlay() override;
};