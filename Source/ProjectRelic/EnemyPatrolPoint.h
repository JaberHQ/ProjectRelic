// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "EnemyPatrolPoint.generated.h"

/***************************************************************************************
 * Type: Class
 *
 * Name: EnemyPatrolPoint 
 *
 * Author: Jaber Ahmed
 *
 * Purpose: Actor for patrol points for AI to follow (Prototype)
 *
 * Functions: N/A
 *
 * References: N/A
 *
 * See Also: SelectEnemyPatrolPoint, EnemyController
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 31/03/2022    JA			 v1.0        Created patrol point
 * 17/05/2022	 JA			 v1.1		 Depreciated, look at CPP_PatrolPoint
 ***************************************************************************************/
UCLASS()
class PROJECTRELIC_API AEnemyPatrolPoint : public ATargetPoint
{
	GENERATED_BODY()
	
};
