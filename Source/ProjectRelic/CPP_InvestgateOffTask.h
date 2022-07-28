// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CPP_InvestgateOffTask.generated.h"

/*****************************************************************************************************
 * Type: Class
 *
 * Name: CPP_InvestgateOffTask
 *
 * Author: Jaber Ahmed
 *
 * Purpose: For AI to stop investigating
 *
 * References: N/A
 *
 * See Also: N/A
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 28/07/2022    JA          v1.0        Created investigate off task
*****************************************************************************************************/
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
