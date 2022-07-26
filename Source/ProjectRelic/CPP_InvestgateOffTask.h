// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CPP_InvestgateOffTask.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRELIC_API UCPP_InvestgateOffTask : public UBTTaskNode
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
