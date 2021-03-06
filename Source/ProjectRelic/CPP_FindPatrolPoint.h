// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CPP_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CPP_FindPatrolPoint.generated.h"

/**********************************************************************************************************************
 * Type: Class
 *
 * Name: CPP_FindPatrolPoint
 *
 * Author: Jaber Ahmed
 *
 * Purpose: AI Task to find patrol point
 *
 * Functions: virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory ) override
 *
 * References: N/A
 *
 * See Also: N/A
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 31/03/2022    JA			 v1.0         Created script to find patrol points and have BB
 * 03/05/2022    JA          v2.0         Created a task and implemented it into behaviour tree
 * 18/05/2022	 JA			 v2.1	      Depreciated, see CPP_FindPatrolPathTask instead
 **********************************************************************************************************************/
UCLASS()
class PROJECTRELIC_API UCPP_FindPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()
		/*******************************************************************************************************************************
		*   Function        : virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory ) override
		*   Purpose         : Execute task to find patrol points
		*   Parameters      : UBehaviorTreeComponent& ownerComp, uint8* nodeMemory
		*   Returns         : EBTNodeResult::Succeeded, EBTNodeResult::Failed
		*   Date altered    : 03/05/2022
		*   Contributors    : Jaber Ahmed
		*   Notes           : N/A
		*   See also        : N/A
		*******************************************************************************************************************************/
	virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory ) override;

};
