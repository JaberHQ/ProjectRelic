// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CPP_ChangeCoverTask.generated.h"

/**********************************************************************************************************************
 * Type: Class
 *
 * Name: CPP_ShootFromCover
 *
 * Author: Jaber Ahmed
 *
 * Purpose: Switch cover points
 *
 * References: N/A
 *
 * See Also: CPP_AIManager, CPP_AIController
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 25/07/2022    JA          v1.1        Task for AI to shoot
***********************************************************************************************************************/
UCLASS()
class PROJECTRELIC_API UCPP_ChangeCoverTask : public UBTTaskNode
{
	GENERATED_BODY()
public:
	/*****************************************************************************************************************************
	*   Function        : virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory ) override
	*   Purpose         : Execute task to investigate Player
	*   Parameters      : UBehaviorTreeComponent& ownerComp, uint8* nodeMemory
	*   Returns         : N/A
	*   Date altered    : 04/08/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
	*****************************************************************************************************************************/
	virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory ) override;
};
