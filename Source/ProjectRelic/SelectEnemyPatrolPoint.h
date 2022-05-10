// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyPatrolPoint.h"
#include "EnemyController.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SelectEnemyPatrolPoint.generated.h"

/*****************************************************************************************************************************
 * Type: Class
 *
 * Name: SelectEnemyPatrolPoint (Prototype)
 *
 * Author: Jaber Ahmed
 *
 * Purpose: BB Task for enemy to select node and walk path
 *
 * Functions: virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory ) override
 *
 * References: Reubs via YouTube, https://www.youtube.com/watch?v=3Z1A825gTA8&list=PLBBe1hvULrciqnr3wdS77c5CLfTeHza0X&index=4
 *				[Accessed: 20th March 2022 | Has been modified]
 *
 * See Also: EnemyController
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 31/03/2022    JA			 1.0         Created script to find patrol points and have BB
 * 03/05/2022	 JA			 1.01		 Depreciated, going to be using CPP_FindPatrolPath
 *****************************************************************************************************************************/
UCLASS()
class PROJECTRELIC_API USelectEnemyPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()
		/********************************************************************************************************************************
		   *   Function        : virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory ) override
		   *   Purpose         : Select enemy patrol point node
		   *   Parameters      : UBehaviorTreeComponent& ownerComp, uint8* nodeMemory
		   *   Returns         : EBTNodeResult::Succeeded, EBTNodeResult::Failed
		   *   Date altered    : 09/04/2022
		   *   Contributors    : Jaber Ahmed
		   *   Notes           : N/A
		   *   See also        : EnemyController
		********************************************************************************************************************************/
		virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory ) override;
};
