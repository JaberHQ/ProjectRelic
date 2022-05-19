// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CPP_AIController.h"
#include "CPP_AIManager.h"
#include "CPP_ChasePlayerTask.generated.h"

/*********************************************************************************************************************
 * Type: Class
 *
 * Name: CPP_ChasePlayerTask
 *
 * Author: Jaber Ahmed
 *
 * Purpose: BT Task for Enemy to chase Player (Version 2 and above of AI Behaviour)
 *
 * Functions: virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory ) override
 *
 * References: N/A
 *
 * See Also: CPP_AIController.h, CPP_AIManager.h
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 02/05/2022    JA          v2.0        Created a task and implemented it into Behaviour Tree
 *********************************************************************************************************************/
UCLASS()
class PROJECTRELIC_API UCPP_ChasePlayerTask : public UBTTaskNode
{
	GENERATED_BODY()
	/*****************************************************************************************************************************
	*   Function        : virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory ) override
	*   Purpose         : Execute task to chase Player
	*   Parameters      : UBehaviorTreeComponent& ownerComp, uint8* nodeMemory
	*   Returns         : N/A
	*   Date altered    : 02/05/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
	*****************************************************************************************************************************/
	virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory ) override;

};
