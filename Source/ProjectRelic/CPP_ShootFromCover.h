// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CPP_ShootFromCover.generated.h"

/**********************************************************************************************************************
 * Type: Class
 *
 * Name: CPP_ShootFromCover
 *
 * Author: Jaber Ahmed
 *
 * Purpose: For AI to shoot player within cover, called within the blackboard
 *
 * Functions: virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory ) override
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
class PROJECTRELIC_API UCPP_ShootFromCover : public UBTTaskNode
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
