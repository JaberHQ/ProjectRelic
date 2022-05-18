// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_PatrolPoint.generated.h"
/*****************************************************************************************************
 * Type: Class
 *
 * Name: CPP_PatrolPoint
 *
 * Author: Jaber Ahmed
 *
 * Purpose: Patrol points for AI to follow (pathing)
 *
 * Functions:  ACPP_PatrolPoint(), FVector GetPatrolPoint( int index ) const;	
 *				int Num() const, virtual void BeginPlay() override, 
 *				virtual void Tick(float DeltaTime) override, TArray<FVector> patrolPoints,	
 *
 * References: MrCxx, UE4 C++ AI Tutorial Series Part 5 Patrolling AI NPC
 *				 https://www.youtube.com/watch?v=99NT6lQ6SJ0&list=PLBBe1hvULrciqnr3wdS77c5CLfTeHza0X&index=31
 *			     [Accessed 17/05/2022 | Modified]
 *
 * See Also: N/A
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 31/03/2022    JA			 v1.0        Created patrol point
 * 17/05/2022    JA          v2.0        A new patrol point from scratch that multiple AI can follow
*****************************************************************************************************/
UCLASS()
class PROJECTRELIC_API ACPP_PatrolPoint : public AActor
{
	GENERATED_BODY()
	
public:
	 /*****************************************************************************
	  *   Function        : ACPP_PatrolPoint()
	  *   Purpose         : Constructor
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 17/05/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A           
	*****************************************************************************/
	ACPP_PatrolPoint();
	/*****************************************************************************
	 *   Function        : FVector GetPatrolPoint( int index ) const
	 *   Purpose         : Return patrol point
	 *   Parameters      : int index 
	 *   Returns         : return patrolPoints[ index ]
	 *   Date altered    : 17/05/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
   *****************************************************************************/
	FVector GetPatrolPoint( int index ) const;
	/*****************************************************************************
	 *   Function        : int Num() const
	 *   Purpose         : Return number of points
	 *   Parameters      : N/A
	 *   Returns         : return patrolPoints.Num()
	 *   Date altered    : 17/05/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
   *****************************************************************************/
	int Num() const;

protected:
	/*****************************************************************************
	 *   Function        : virtual void BeginPlay() override
	 *   Purpose         : Begin play event
	 *   Parameters      : N/A
	 *   Returns         : N/A
	 *   Date altered    : 17/05/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
   *****************************************************************************/
	virtual void BeginPlay() override;

public:	
	/*****************************************************************************
	 *   Function        : virtual void Tick(float DeltaTime) override
	 *   Purpose         : Tick event
	 *   Parameters      : float DeltaTime
	 *   Returns         : N/A
	 *   Date altered    : 17/05/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
   *****************************************************************************/
	virtual void Tick( float DeltaTime ) override;

private:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "AI", meta = ( MakeEditWidget = "true", AllowPrivateAccess = "true" ) )
		TArray<FVector> patrolPoints; // Array of patrol points


};
