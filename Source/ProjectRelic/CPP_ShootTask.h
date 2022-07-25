// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CPP_ShootTask.generated.h"

/**********************************************************************************************************************
 * Type: Class
 *
 * Name: CPP_ShootTask
 *
 * Author: Jaber Ahmed
 *
 * Purpose: For AI to shoot player, called within the blackboard
 *
 * Functions: virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory ) override
 *
 * References: N/A
 *
 * See Also: N/A
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 15/07/2022    JA          v1.1        Task for AI to shoot
 * 25/07/2022	 JA			 v1.2		 Replaced for CPP_ShootFromCover
***********************************************************************************************************************/
UCLASS()
class PROJECTRELIC_API UCPP_ShootTask : public UBTTaskNode
{
	GENERATED_BODY()
	/*****************************************************************************************************************************
		*   Function        : virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory ) override
		*   Purpose         : Execute task to Shoot Player
		*   Parameters      : UBehaviorTreeComponent& ownerComp, uint8* nodeMemory
		*   Returns         : N/A
		*   Date altered    : 23/06/2022
		*   Contributors    : Jaber Ahmed
		*   Notes           : N/A
		*   See also        : N/A
		*****************************************************************************************************************************/
		virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory ) override;
};
