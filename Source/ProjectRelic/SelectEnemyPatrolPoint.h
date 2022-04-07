// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyPatrolPoint.h"
#include "EnemyController.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SelectEnemyPatrolPoint.generated.h"

/***************************************************************************************
 * Type: Class
 *
 * Name: SelectEnemyPatrolPoint
 *
 * Author: Jaber Ahmed
 *
 * Purpose:
 *
 * Functions:
 *
 * References: Reubs via YouTube, https://www.youtube.com/watch?v=3Z1A825gTA8&list=PLBBe1hvULrciqnr3wdS77c5CLfTeHza0X&index=4
 *				[Accessed: 20th March 2022 | Has been modified]
 *
 * See Also:
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 31/03/2022    JA			 1.0     ----------------------------------------------
 ***************************************************************************************/
UCLASS()
class PROJECTRELIC_API USelectEnemyPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()
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
		virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory ) override;
};
