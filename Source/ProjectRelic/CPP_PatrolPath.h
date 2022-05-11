// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_PatrolPath.generated.h"
/**************************************************************************************
 * Type: Class
 *
 * Name: CPP_PatrolPath
 *
 * Author: Jaber Ahmed
 *
 * Purpose: Patrol path of AI (Version 2 and above)
 * 
 * Functions: ACPP_PatrolPath(), virtual void BeginPlay() override,
 *			  virtual void Tick(float DeltaTime) override, 	
 *
 * References: N/A
 *
 * See Also: EnemyPatrolPoint.h
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 02/05/2022    JA          v2.0        Created an actor, needs further implementation
 **************************************************************************************/
UCLASS()
class PROJECTRELIC_API ACPP_PatrolPath : public AActor
{
	GENERATED_BODY()
private:
public:	
	/*****************************************************************************
	*   Function        : ACPP_PatrolPath()
	*   Purpose         : Constructor
	*   Parameters      : N/A
	*   Returns         : N/A
	*   Date altered    : 02/05/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
	*****************************************************************************/
	ACPP_PatrolPath();
	/*****************************************************************************
	*   Function        : virtual void BeginPlay() override
	*   Purpose         : BeginPlay event when the game starts or when spawned
	*   Parameters      : N/A
	*   Returns         : N/A
	*   Date altered    : 02/05/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
	*****************************************************************************/
	virtual void BeginPlay() override;
	/*****************************************************************************
	*   Function        : virtual void Tick(float DeltaTime) override
	*   Purpose         : Tick event for every frame
	*   Parameters      : N/A
	*   Returns         : N/A 
	*   Date altered    : 02/05/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
	*****************************************************************************/
	virtual void Tick(float DeltaTime) override;


};
