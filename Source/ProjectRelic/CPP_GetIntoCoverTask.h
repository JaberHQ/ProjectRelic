// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CPP_GetIntoCoverTask.generated.h"

/*****************************************************************************************************
 * Type: Class
 *
 * Name: CPP_GetIntoCoverTask
 *
 * Author: Jaber Ahmed
 *
 * Purpose: AI to get to cover using BT
 *
 * References: N/A
 *
 * See Also: BT_AssaultSoldier
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 28/07/2022    JA          v1.0		 Created a task to move to cover
*****************************************************************************************************/
UCLASS()
class PROJECTRELIC_API UCPP_GetIntoCoverTask : public UBTTaskNode
{
	GENERATED_BODY()
public:
	/*****************************************************************************************************************************
	*   Function        : virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory ) override
	*   Purpose         : Execute task to investigate Player
	*   Parameters      : UBehaviorTreeComponent& ownerComp, uint8* nodeMemory
	*   Returns         : N/A
	*   Date altered    : 23/06/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
	*****************************************************************************************************************************/
	virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory ) override;
};
